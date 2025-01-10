/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: badriano <badriano@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 08:31:04 by gecarval          #+#    #+#             */
/*   Updated: 2025/01/09 08:13:40 by gecarval         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/minishell.h"

int	ft_crtl_c(int value)
{
	static int	status = 0;

	if (value != -1)
		status = value;
	return (status);
}

t_fd	*ft_fd_address(t_fd *fd)
{
	static t_fd	*fd_ad;

	if (fd != NULL)
		fd_ad = fd;
	return (fd_ad);
}

t_shell	*ft_shell_address(t_shell *shell)
{
	static t_shell	*shell_ad;

	if (shell != NULL)
		shell_ad = shell;
	return (shell_ad);
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
		shell.status = ft_crtl_c(-1);
		if (shell.line == NULL)
			write(1, "exit\n", 5);
		if (shell.line == NULL)
			break ;
		if (shell.line[0] != '\0')
		{
			add_history(shell.line);
			parse_line(&shell);
			if (shell.heredoc_exitstatus == 130)
				shell.heredoc_exitstatus = 0;
			exec_cmd(&shell);
		}
		free(shell.line);
	}
	return (ft_free_all(&shell, false, true));
}
