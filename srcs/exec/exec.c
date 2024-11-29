/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gecarval <gecarval@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 08:40:26 by gecarval          #+#    #+#             */
/*   Updated: 2024/11/29 08:10:18 by gecarval         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ft_exec_if_pipe(t_cmd *cmd, t_shell *shell)
{
	pid_t	pid;

	printf("pipe exec\n");
	signal(SIGINT, ft_signal_hand);
	while (cmd != NULL)
	{
		if (cmd->next != NULL)
			pipe(shell->pipe_fd);
		pid = ft_fork(shell);
		if (pid == 0)
		{
		  signal(SIGQUIT, SIG_DFL);
			// OUTPUT FD
			if (cmd->fd.fd_out != STDOUT_FILENO)
				ft_dup2(cmd->fd.fd_out, STDOUT_FILENO, shell, NULL);
			else if (cmd->next != NULL)
				ft_dup2(shell->pipe_fd[1], STDOUT_FILENO, shell, NULL);
			// SET INPUT FD AND CLOSE INPUT PIPE FD
      if (cmd->fd.fd_in != STDIN_FILENO)
        ft_dup2(cmd->fd.fd_in, STDIN_FILENO, shell, NULL);
			if (cmd->next != NULL)
		  	close(shell->pipe_fd[0]);
			// EXEC ON BUILTIN
			if (ft_exec_on_builtin(cmd, shell) >= 0)
			{
				if (cmd->next != NULL)
  				close(shell->pipe_fd[1]);
				ft_free_all(shell);
				exit(shell->status);
			}
			// EXEC ON PATH IF NOT A BUILTIN
			ft_exec_on_path(shell, cmd);
			if (cmd->next != NULL)
			  close(shell->pipe_fd[1]);
			ft_free_all(shell);
			exit(shell->status);
		}
		// INPUT FD FOR NEXT CMD IF PIPE
		if (cmd->next != NULL && cmd->next->fd.fd_in == STDIN_FILENO)
			ft_dup2(shell->pipe_fd[0], STDIN_FILENO, shell, NULL);
		// CLOSE PIPE FD AND SET FOR NEXT CMD
		if (cmd->next != NULL)
	  	close(shell->pipe_fd[1]);
		if (cmd->next != NULL)
  		close(shell->pipe_fd[0]);
		cmd = cmd->next;
	}
	// WAIT LAST PROCESS
	waitpid(pid, &shell->status, 0);
	if (WIFEXITED(shell->status))
		shell->status = WEXITSTATUS(shell->status);
	else if (shell->status == 2)
		shell->status = (shell->status << 6) + 2;
	ft_free_all(shell);
	return (shell->status);
}

int	ft_normal_exec(t_cmd *cmd, t_shell *shell)
{
	printf("normal exec\n");
	signal(SIGQUIT, SIG_DFL);
	if (cmd->fd.fd_out != STDOUT_FILENO)
		ft_dup2(cmd->fd.fd_out, STDOUT_FILENO, shell, NULL);
	if (cmd->fd.fd_in != STDIN_FILENO)
		ft_dup2(cmd->fd.fd_in, STDIN_FILENO, shell, NULL);
	if (ft_exec_on_builtin(cmd, shell) >= 0)
	{
		ft_free_all(shell);
		exit(shell->status);
	}
	ft_exec_on_path(shell, cmd);
	ft_free_all(shell);
	return (shell->status);
}

// This function executes the command in the parent process
// If necessary to execute on parent like cd or exit and a single command
// Then it executes the command on the child process if it is on PATH
// If the command is a pipe, in another child process to avoid EOF
void	exec_cmd(t_shell *shell)
{
	pid_t	pid;

	if (shell->cmd->type == EXEC && ft_exec_on_parent(shell->cmd, shell) >= 0)
		return ;
	pid = ft_fork(shell);
	if (pid == 0)
	{
		if (shell->cmd->type == PIPE)
			exit(ft_exec_if_pipe(shell->cmd, shell));
		else
			exit(ft_normal_exec(shell->cmd, shell));
	}
	signal(SIGINT, ft_signal_hand);
	waitpid(pid, &shell->status, 0);
	if (WIFEXITED(shell->status))
		shell->status = WEXITSTATUS(shell->status);
	else if (shell->status == 2)
		shell->status = (shell->status << 6) + 2;
}
