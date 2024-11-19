/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gecarval <gecarval@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 08:31:04 by gecarval          #+#    #+#             */
/*   Updated: 2024/11/19 16:38:15 by gecarval         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/minishell.h"

void	ft_signal_handler(int signum)
{
	(void)signum;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

// This start by initializing shell data struct
// Then it reads the line from the user
// If the line is empty, it breaks the loop
// If the line is not empty, it adds the line to the history
// Then it parses the line
// Then it prints the command structure List
// Then it executes the command
// Then it frees the command structure List
int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;

	(void)argc;
	(void)argv;
	signal(SIGQUIT, SIG_IGN);
	ft_init_shell(&shell, envp);
	while (1)
	{
		signal(SIGINT, ft_signal_handler);
		shell.line = ft_limit_buffer(readline(PROMPT));
		if (shell.line == NULL)
			write(1, "exit\n", 5);
		if (shell.line == NULL)
			break ;
		add_history(shell.line);
		parse_line(&shell);
		exec_cmd(&shell);
		free_cmd(&shell.cmd);
		if (shell.line != NULL)
			free(shell.line);
		shell.fd_in = STDIN_FILENO;
		shell.fd_out = STDOUT_FILENO;
	}
	ft_free_all(&shell);
	return (0);
}
