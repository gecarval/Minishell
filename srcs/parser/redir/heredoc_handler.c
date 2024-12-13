/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_handler.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gecarval <gecarval@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 09:11:23 by gecarval          #+#    #+#             */
/*   Updated: 2024/12/13 16:09:44 by gecarval         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

static void	signal_heredoc(int signum)
{
	t_fd	*fds;

	(void)signum;
	fds = ft_fd_address(NULL);
	close(fds->fd_in);
	free(fds->filename_in);
	ft_free_all(ft_shell_address(NULL), false);
	exit(130);
}

static void	ft_define_heredoc_signal(t_fd *fds)
{
	ft_fd_address(fds);
	signal(SIGINT, signal_heredoc);
	signal(SIGQUIT, SIG_IGN);
}

static void	ft_exec_heredoc_on_child(t_fd *fds, t_shell *shell)
{
	char	*line;

	ft_shell_address(shell);
	fds->fd_in = open(".heredoc_tmp", O_CREAT | O_WRONLY | O_TRUNC, 0644);
	while (1)
	{
		ft_define_heredoc_signal(fds);
		line = readline("heredoc> ");
		if (line == NULL)
			break ;
		if (ft_strncmp(line, fds->filename_in,
				ft_strlen(fds->filename_in)) == 0)
		{
			free(line);
			break ;
		}
		ft_expand_sign_matrix(&line, shell, 1);
		write(fds->fd_in, line, ft_strlen(line));
		write(fds->fd_in, "\n", 1);
		free(line);
	}
	close(fds->fd_in);
	free(fds->filename_in);
	ft_free_all(shell, true);
	exit(0);
}

void	ft_heredoc_handler(t_fd *fds, t_shell *shell)
{
	int		status;
	pid_t	pid;

	pid = ft_fork(shell);
	if (pid == 0)
		ft_exec_heredoc_on_child(fds, shell);
	waitpid(pid, &status, 0);
	if (WIFEXITED(status) == true)
		status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status) == true)
		status = WTERMSIG(status) + 128;
	if (status == 130)
	{
		shell->status = ft_crtl_c(130);
		shell->heredoc_exitstatus = 130;
		free_cmd(&shell->cmd);
	}
	else
		fds->fd_in = open(".heredoc_tmp", O_RDONLY);
}
