/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gecarval <gecarval@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 08:39:32 by gecarval          #+#    #+#             */
/*   Updated: 2024/10/29 13:16:21 by gecarval         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// This function gets the flags of the command
// It returns NULL if the command is echo and the first argument is not a flag
// It returns NULL if the first argument is not a flag
// It returns NULL if there are no flags
// It allocates memory for the flags
// It gets the flags and stores them in the flags array
// It returns the flags array
char	**ft_get_flags(t_cmd *new, char **args)
{
	char	**flags;
	int		i;
	int		flag_count;

	i = 0;
	flag_count = 0;
	if (args[0] == NULL)
		return (NULL);
	if (ft_strncmp(new->cmd, "echo", 4) == 0 && args[0][0] != '-')
		return (NULL);
	while (args[++i] != NULL)
		if (args[i][0] == '-')
			flag_count++;
	if (flag_count == 0)
		return (NULL);
	flags = (char **)malloc(sizeof(char *) * (flag_count + 1));
	if (!flags)
		return (NULL);
	i = -1;
	flag_count = 0;
	while (args[++i] != NULL)
		if (args[i][0] == '-')
			flags[flag_count++] = ft_strdup(args[i]);
	return (flags);
}

char	*ft_get_cmdoutput(char **args)
{
	char	*str;
	char	*tmp;
	int		i;

	i = 0;
	str = NULL;
	while (args[i] != NULL)
	{
		if (args[i][0] == '-' && flag == 0)
			continue ;
		else
		{
			flag = 1;
			tmp = ft_strjoin(str, args[i]);
			free(str);
			str = ft_strjoin(tmp, " ");
			free(tmp);
		}
	}
	return (str);
}

// This function adds the arguments and the output to the command structure
// It counts the number of arguments
// It gets the flags using the ft_get_flags function
// It gets the output using the ft_get_cmdoutput function
void	add_args_and_output(t_cmd *new, char **args)
{
	int	i;

	i = 0;
	while (args[i] != NULL)
		i++;
	shell->new->argc = i;
	shell->new->args = ft_get_flags(shell, args);
	shell->new->str_to_print = ft_get_cmdoutput(args);
}

void	ft_handle_ispipe(t_cmd *new, int is_pipe)
{
	if (is_pipe == 1)
	{
		new->type = PIPE;
		new->fd_in = 0;
		new->fd_out = 1;
	}
	else
	{
		new->type = EXEC;
		new->fd_in = 0;
		new->fd_out = 1;
	}
}

void	add_cmd(t_shell *shell, char **args, int is_pipe)
{
	shell->new = NULL;
	shell->new = (t_cmd *)malloc(sizeof(t_cmd));
	if (!shell->new)
		return ;
	shell->new->cmd = args[0];
	add_args_and_output(shell, args + 1);
	shell->new->next = NULL;
	ft_handle_ispipe(shell->new, is_pipe);
	if (!shell->cmd)
		shell->cmd = shell->new;
	else
	{
		last = shell->cmd;
		while (last->next != NULL)
			last = last->next;
		last->next = new;
	}
}

// This function parses the line and adds the commands to the command structure List
// It splits the line by pipes and then by spaces using the ft_split function and ft_espur_str
// It then adds the command to the command structure List
// It frees the args and the new_line
void	parse_line(t_shell *shell)
{
	char	**cmds;
	char	**args;
	char	*new_line;
	int		i;

	ft_check_quotes(shell->line);
	cmds = ft_split(shell->line, '|');
	i = 0;
	while (cmds[i])
	{
		new_line = ft_espur_str(cmds[i]);
		args = ft_split(new_line , ' ');
		free(new_line);
		add_cmd(shell, args, ft_is_pipe(shell->line));
		i++;
	}
}
