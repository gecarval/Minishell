/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gecarval <gecarval@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 08:39:32 by gecarval          #+#    #+#             */
/*   Updated: 2024/10/21 09:34:51 by gecarval         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void add_args_and_output(t_cmd **cmd, char **args)
{
    int i;

    i = 0;
    while (args[i] != NULL)
        i++;
    (*cmd)->argc = i;
    (*cmd)->args = args;
    (*cmd)->str_to_print = ft_strjoin((*cmd)->cmd, " ");
}

void	add_cmd(t_cmd **cmd, char **args)
{
	t_cmd	*new;
	t_cmd	*last;

	new = malloc(sizeof(t_cmd));
	if (!new)
		return ;
	new->cmd = args[0];
    add_args_and_output(&new, args + 1);
	new->type = EXEC;
	new->fd_in = 0;
	new->fd_out = 1;
	new->next = NULL;
	if (!*cmd)
		*cmd = new;
	else
	{
		last = *cmd;
		while (last->next != NULL)
			last = last->next;
		last->next = new;
	}
}

void	parse_line(char *line, t_cmd **cmd)
{
	char	**cmds;
	char	**args;
	int		i;

	cmds = ft_split(line, '|');
	i = 0;
	while (cmds[i])
	{
		args = ft_split(ft_espur_str(cmds[i]), ' ');
		add_cmd(cmd, args);
		i++;
	}
}
