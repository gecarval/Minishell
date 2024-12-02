/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gecarval <gecarval@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 08:31:04 by gecarval          #+#    #+#             */
/*   Updated: 2024/12/02 11:44:42 by gecarval         ###   ########.fr       */
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
		i = -1;
		while (tmp->args[++i] != NULL)
			printf("arg[%d]: %s\n", i, tmp->args[i]);
		printf("argc: %d\n", tmp->argc);
		printf("type: %d\n", tmp->type);
		printf("fd_in: %d\n", tmp->fd.fd_in);
		printf("fd_out: %d\n", tmp->fd.fd_out);
		if (tmp->fd.filename_in != NULL)
			printf("filename_in: %s\n", tmp->fd.filename_in);
		else
			printf("filename_in: %s\n", "(null)");
		if (tmp->fd.filename_out != NULL)
			printf("filename_out: %s\n", tmp->fd.filename_out);
		else
			printf("filename_out: %s\n", "(null)");
		tmp = tmp->next;
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;

	(void)argc;
	(void)argv;
	ft_init_shell(&shell, envp);
	while (1)
	{
		signal(SIGINT, ft_signal_handler);
		shell.line = ft_limit_buffer(readline(PROMPT));
		if (shell.line == NULL)
			write(1, "exit\n", 5);
		if (shell.line == NULL)
			break ;
		if (shell.line[0] != '\0')
		{
			add_history(shell.line);
			parse_line(&shell);
			exec_cmd(&shell);
			free_cmd(&shell.cmd);
		}
		free(shell.line);
	}
	return (ft_free_all(&shell));
}
