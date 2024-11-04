/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gecarval <gecarval@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 08:39:32 by gecarval          #+#    #+#             */
/*   Updated: 2024/11/04 10:58:30 by gecarval         ###   ########.fr       */
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
	while (args[i] != NULL)
		if (args[i++][0] == '-')
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
	flags[flag_count] = NULL;
	return (flags);
}

// This function gets the output of the command
// It concatenates the arguments into a single string
// It skips the first argument if it is a flag
// It returns the string
char	*ft_get_cmdoutput(char **args)
{
	char	*str;
	char	*tmp;
	int		flag;
	int		i;

	i = -1;
	flag = 0;
	str = ft_strdup("");
	if (args[0] == NULL)
		return (str);
	while (args[++i] != NULL)
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

// ft_strdup but for matrix
char	**ft_matdup(char **mat)
{
	char	**new;
	int		i;

	i = 0;
	if (mat == NULL)
		return (NULL);
	while (mat[i] != NULL)
		i++;
	new = (char **)malloc(sizeof(char *) * (i + 1));
	i = -1;
	while (mat[++i] != NULL)
		new[i] = ft_strdup(mat[i]);
	new[i] = NULL;
	return (new);
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
	new->argc = i;
	new->args = ft_matdup(args);
}

// This function checks if the line contains a pipe
// It returns 1 if the line contains a pipe
// This serves to facilitate when executing the command
void	ft_handle_ispipe(t_cmd *new, int is_pipe)
{
	if (is_pipe == 1)
		new->type = PIPE;
	else
		new->type = EXEC;
}

// This function adds the command to the command structure List
// It allocates memory for the new command
// It adds the command to the command structure List
// If the command structure List is empty, it sets the new command as the head of the List
// If the command structure List is not empty, it sets the new command as the last command of the List
void	add_cmd(t_shell *shell, char **args, int is_pipe)
{
	t_cmd	*new;
	t_cmd	*last;

	new = (t_cmd *)malloc(sizeof(t_cmd));
	if (new == NULL)
		return ;
	new->next = NULL;
	new->cmd = ft_strdup(args[0]);
	ft_handle_ispipe(new, is_pipe);
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

// This function parses the line and adds the commands to the command structure List
// It splits the line by pipes and then by spaces using the ft_split function and ft_espur_str
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
	ft_check_quotes(shell->line);
	cmds = ft_split(shell->line, '|');
	while (cmds[i])
	{
		args = ft_parser_split(cmds[i], " ");
		add_cmd(shell, args, ft_is_pipe(shell->line));
		ft_free_args(args);
		free(cmds[i]);
		i++;
	}
	free(cmds);
}
