/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gecarval <gecarval@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 08:31:04 by gecarval          #+#    #+#             */
/*   Updated: 2024/10/21 09:22:28 by gecarval         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/minishell.h"

void ft_print_cmd(t_cmd *cmd)
{
	t_cmd	*tmp;
	int		i;

	tmp = cmd;
	while (tmp != NULL)
	{
		printf("cmd: %s\n", tmp->cmd);
		i = 0;
		while (tmp->args[i] != NULL)
		{
			printf("arg[%d]: %s\n", i, tmp->args[i]);
			i++;
		}
		printf("argc: %d\n", tmp->argc);
		printf("type: %d\n", tmp->type);
		printf("fd_in: %d\n", tmp->fd_in);
		printf("fd_out: %d\n", tmp->fd_out);
		tmp = tmp->next;
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_cmd	*cmd;
	char	*line;

	(void)argc;
	(void)argv;
	cmd = NULL;
	while (1)
	{
		line = ft_limit_buffer(readline(PROMPT));
		if (!line)
			break ;
		if (*line)
			add_history(line);
		parse_line(line, &cmd);
		ft_print_cmd(cmd);
		exec_cmd(cmd, envp);
		if (cmd != NULL)
			free_cmd(&cmd);
		if (line != NULL)
			free(line);
	}
	return (0);
}
