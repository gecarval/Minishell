/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gecarval <gecarval@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 08:40:26 by gecarval          #+#    #+#             */
/*   Updated: 2024/10/31 12:30:48 by gecarval         ###   ########.fr       */
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

void	ft_execve(char *bin, char **args, char **env, t_shell *shell)
{
	if (execve(bin, args, env) == -1)
	{
		printf("minishell: %s: command not found\n", shell->cmd->cmd);
		ft_free_all(shell);
		free(bin);
		exit(1);
	}
}

void	ft_exec_child(t_shell *shell)
{
	char	*bin_route;

	bin_route = ft_strjoin("/bin/", shell->cmd->cmd);
	if (shell->cmd->type == EXEC)
	{
		ft_execve(bin_route, shell->cmd->args, shell->envp, shell);
	}
	else if (shell->cmd->type == PIPE)
	{
		if (dup2(shell->cmd->fd_in, 0) == -1)
		{
			printf("minishell: dup2 failed\n");
			free(bin_route);
			ft_free_all(shell);
			exit(1);
		}
		if (dup2(shell->cmd->fd_out, 1) == -1)
		{
			printf("minishell: dup2 failed\n");
			free(bin_route);
			ft_free_all(shell);
			exit(1);
		}
		ft_execve(shell->cmd->cmd, shell->cmd->args, shell->envp, shell);
	}
	free(bin_route);
}

// This function executes the command
// It forks the process
// If the pid is 0, it executes the child process
// If the pid is not 0, it waits for the child process to finish
// Then it goes to the next command
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
			exit(0);
		}
		else
		{
			waitpid(pid, &status, 0);
			cmd = cmd->next;
		}
	}
}
