/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gecarval <gecarval@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 08:39:32 by gecarval          #+#    #+#             */
/*   Updated: 2024/10/29 13:13:11 by gecarval         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	**ft_get_flags(t_cmd *new, char **args)
{
	char	**flags;
	int		i;
	int		flag_count;

	i = 0;
	flag_count = 0;
	if (args[1] == NULL)
		return (NULL);
	if (ft_strncmp(new->cmd, "echo", 4) == 0 && args[1][0] != '-')
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
	flags[flag_count] = NULL;
	return (flags);
}

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
			flag = 0;
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

void	add_args_and_output(t_cmd *new, char **args)
{
	int	i;

	i = 0;
	while (args[i] != NULL)
		i++;
	new->argc = i;
	new->args = ft_get_flags(new, args);
	new->str_to_print = ft_get_cmdoutput(args);
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
	t_cmd	*new;
	t_cmd	*last;

	new = (t_cmd *)malloc(sizeof(t_cmd));
	if (!new)
		return ;
	new->next = NULL;
	new->cmd = ft_strdup(args[0]);
	add_args_and_output(new, args + 1);
	ft_handle_ispipe(new, is_pipe);
	if (!shell->cmd)
		shell->cmd = new;
	else
	{
		last = shell->cmd;
		while (last->next != NULL)
			last = last->next;
		last->next = new;
	}
}

void	parse_line(t_shell *shell)
{
	char	**cmds;
	char	**args;
	char	*new_line;
	int		i;

	i = 0;
	if (!shell->line)
		return ;
	ft_check_quotes(shell->line);
	cmds = ft_split(shell->line, '|');
	while (cmds[i])
	{
		new_line = ft_espur_str(cmds[i]);
		args = ft_split(new_line, ' ');
		add_cmd(shell, args, ft_is_pipe(shell->line));
		ft_free_args(args);
		free(new_line);
		free(cmds[i]);
		i++;
	}
	free(cmds);
}
