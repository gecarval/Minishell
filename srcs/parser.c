/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gecarval <gecarval@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 08:39:32 by gecarval          #+#    #+#             */
/*   Updated: 2024/11/07 16:31:31 by gecarval         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// This function adds the arguments
// and the output to the command structure
// It counts the number of arguments
// It gets the flags using the ft_get_flags function
// It gets the output using the ft_get_cmdoutput function
void	add_args_and_output(t_cmd *new, char **args)
{
	int	i;

	i = 0;
	while (args[i] != NULL)
		i++;
	new->argc = i;
	new->args = ft_matdup(args);
}

// This function adds the command to the command structure List
// It allocates memory for the new command
// It adds the command to the command structure List
// If the command structure List is empty,
//	it sets the new command as the head of the List
// If the command structure List is not empty,
//	it sets the new command as the last command of the List
void	add_cmd(t_shell *shell, char **args, int is_pipe)
{
	t_cmd	*new;
	t_cmd	*last;

	new = (t_cmd *)malloc(sizeof(t_cmd));
	if (new == NULL)
		return ;
	new->next = NULL;
	new->cmd = ft_strdup(args[0]);
	if (is_pipe == 1)
		new->type = PIPE;
	else
		new->type = EXEC;
	add_args_and_output(new, args);
	if (shell->cmd == NULL)
		shell->cmd = new;
	else
	{
		last = shell->cmd;
		while (last->next != NULL)
			last = last->next;
		last->next = new;
	}
}

// This function parses the line and adds the commands
// to the command structure List
// It splits the line by pipes and then by spaces using
// the ft_split function and ft_espur_str
// It then adds the command to the command structure List
// It frees the args and the new_line
void	parse_line(t_shell *shell)
{
	char	**cmds;
	char	**args;
	int		i;

	i = 0;
	if (!shell->line)
		return ;
	if (ft_check_unvalid(shell->line) == 1)
		return ;
	cmds = ft_split(shell->line, '|');
	while (cmds[i])
	{
		args = ft_parser_split(cmds[i], " ", shell);
		add_cmd(shell, args, ft_is_pipe(shell->line));
		ft_free_args(args);
		free(cmds[i]);
		i++;
	}
	free(cmds);
}
