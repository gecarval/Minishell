/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gecarval <gecarval@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 08:40:26 by gecarval          #+#    #+#             */
/*   Updated: 2024/10/23 09:39:53 by gecarval         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// This function forks the process
// It returns the pid of the child process if it was successful
// It prints an error message and frees the shell data if it fails
pid_t	ft_fork(t_shell *shell)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
	{
		printf("minishell: fork failed\n");
		ft_free_all(shell);
		exit(1);
	}
	return (pid);
}

void	ft_exec_child(t_shell *shell)
{
	if (shell->cmd->type == EXEC)
	{
		if (execve(shell->cmd->cmd, shell->cmd->args, shell->envp) == -1)
		{
			printf("minishell: %s: command not found\n", shell->cmd->cmd);
			ft_free_all(shell);
			exit(1);
		}
	}
	else if (shell->cmd->type == PIPE)
	{
		if (dup2(shell->cmd->fd_in, 0) == -1)
		{
			printf("minishell: dup2 failed\n");
			ft_free_all(shell);
			exit(1);
		}
		if (dup2(shell->cmd->fd_out, 1) == -1)
		{
			printf("minishell: dup2 failed\n");
			ft_free_all(shell);
			exit(1);
		}
		if (execve(shell->cmd->cmd, shell->cmd->args, shell->envp) == -1)
		{
			printf("minishell: %s: command not found\n", shell->cmd->cmd);
			ft_free_all(shell);
			exit(1);
		}
	}
}

// This function executes the command
// It forks the process
// If the pid is 0, it executes the child process
// If the pid is not 0, it waits for the child process to finish
void	exec_cmd(t_shell *shell)
{
	t_cmd	*cmd;
	pid_t	pid;
	int		status;

	cmd = shell->cmd;
	while (cmd != NULL)
	{
		pid = ft_fork(shell);
		if (pid == 0)
		{
			ft_exec_child(shell);
			ft_free_all(shell);
		}
		else
		{
			waitpid(pid, &status, 0);
			cmd = cmd->next;
		}
	}
}
