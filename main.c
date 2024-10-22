/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gecarval <gecarval@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 08:31:04 by gecarval          #+#    #+#             */
/*   Updated: 2024/10/22 08:53:42 by gecarval         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/minishell.h"

void	ft_print_cmd(t_cmd *cmd)
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
		printf("str_to_print: %s\n", tmp->str_to_print);
		printf("type: %d\n", tmp->type);
		printf("fd_in: %d\n", tmp->fd_in);
		printf("fd_out: %d\n", tmp->fd_out);
		tmp = tmp->next;
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;

	(void)argc;
	(void)argv;
	shell.envp = envp;
	shell.line = NULL;
	shell.cmd = NULL;
	while (1)
	{
		shell.line = ft_limit_buffer(readline(PROMPT));
		if (!shell.line)
			break ;
		else if (*shell.line)
			add_history(shell.line);
		parse_line(&shell);
		ft_print_cmd(shell.cmd);
		exec_cmd(&shell);
		if (shell.cmd != NULL)
			free_cmd(&shell.cmd);
		if (shell.line != NULL)
			free(shell.line);
	}
	return (0);
}
