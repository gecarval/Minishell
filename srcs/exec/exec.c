/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gecarval <gecarval@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 08:40:26 by gecarval          #+#    #+#             */
/*   Updated: 2024/12/14 19:07:47 by gecarval         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	ft_child_pipe(t_cmd *cmd, t_shell *shell)
{
	signal(SIGQUIT, SIG_DFL);
	if (cmd->fd.fd_out != STDOUT_FILENO)
		ft_dup2(cmd->fd.fd_out, STDOUT_FILENO, shell, cmd->fd.filename_out);
	else if (cmd->next != NULL)
		ft_dup2(shell->pipe_fd[1], STDOUT_FILENO, shell, NULL);
	if (cmd->fd.fd_in != STDIN_FILENO)
		ft_dup2(cmd->fd.fd_in, STDIN_FILENO, shell, cmd->fd.filename_in);
	if (cmd->next != NULL)
		close(shell->pipe_fd[0]);
	if (ft_exec_on_builtin(cmd, shell) >= 0)
	{
		if (cmd->next != NULL)
			close(shell->pipe_fd[1]);
		ft_free_all(shell, true);
		exit(shell->status);
	}
	ft_exec_on_path(shell, cmd);
	if (cmd->next != NULL)
		close(shell->pipe_fd[1]);
	ft_free_all(shell, true);
	exit(shell->status);
}

static void	ft_parent_pipe(t_cmd *cmd, t_shell *shell)
{
	int	i;

	if (cmd->next != NULL && cmd->next->fd.fd_in == STDIN_FILENO)
		ft_dup2(shell->pipe_fd[0], STDIN_FILENO, shell, NULL);
	if (cmd->next != NULL)
		close(shell->pipe_fd[1]);
	if (cmd->next != NULL)
		close(shell->pipe_fd[0]);
	i = 0;
	while (i < 100000)
		i++;
}

int	ft_exec_if_pipe(t_cmd *cmd, t_shell *shell)
{
	pid_t	pid;

	signal(SIGINT, ft_signal_hand);
	while (cmd != NULL)
	{
		if (cmd->next != NULL)
			pipe(shell->pipe_fd);
		pid = fork();
		if (pid == 0)
			ft_child_pipe(cmd, shell);
		ft_parent_pipe(cmd, shell);
		cmd = cmd->next;
	}
	waitpid(pid, &shell->status, 0);
	if (WIFEXITED(shell->status) == true)
		shell->status = WEXITSTATUS(shell->status);
	else if (WIFSIGNALED(shell->status) == true)
		shell->status = 128 + WTERMSIG(shell->status);
	pid = -1;
	while (++pid < ft_lstsize_cmd(shell->cmd))
		wait(NULL);
	ft_crtl_c(shell->status);
	ft_free_all(shell, true);
	return (shell->status);
}

int	ft_normal_exec(t_cmd *cmd, t_shell *shell)
{
	signal(SIGQUIT, SIG_DFL);
	if (cmd->fd.fd_out != STDOUT_FILENO)
		ft_dup2(cmd->fd.fd_out, STDOUT_FILENO, shell, cmd->fd.filename_out);
	if (cmd->fd.fd_in != STDIN_FILENO)
		ft_dup2(cmd->fd.fd_in, STDIN_FILENO, shell, cmd->fd.filename_in);
	if (ft_exec_on_builtin(cmd, shell) >= 0)
	{
		ft_free_all(shell, true);
		exit(shell->status);
	}
	ft_exec_on_path(shell, cmd);
	ft_free_all(shell, true);
	return (shell->status);
}

// This function executes the command in the parent process
// If necessary to execute on parent like cd or exit and a single command
// Then it executes the command on the child process if it is on PATH
// If the command is a pipe, in another child process to avoid EOF
void	exec_cmd(t_shell *shell)
{
	pid_t	pid;

	if (shell->cmd == NULL || shell->cmd->cmd == NULL)
		return ;
	if (shell->cmd->type == EXEC && ft_exec_on_parent(shell->cmd, shell) >= 0)
	{
		free_cmd(&shell->cmd);
		return ;
	}
	pid = fork();
	if (pid == 0)
	{
		if (shell->cmd->type == EXEC)
			exit(ft_normal_exec(shell->cmd, shell));
		else
			exit(ft_exec_if_pipe(shell->cmd, shell));
	}
	signal(SIGINT, ft_signal_hand);
	waitpid(pid, &shell->status, 0);
	if (WIFEXITED(shell->status) == true)
		shell->status = WEXITSTATUS(shell->status);
	else if (WIFSIGNALED(shell->status) == true)
		shell->status = 128 + WTERMSIG(shell->status);
	ft_crtl_c(shell->status);
	free_cmd(&shell->cmd);
}
