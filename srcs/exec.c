/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gecarval <gecarval@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 08:40:26 by gecarval          #+#    #+#             */
/*   Updated: 2024/10/29 13:16:37 by gecarval         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

pid_t	ft_fork(void)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
	{
		printf("minishell: fork failed\n");
		exit(1);
	}
	return (pid);
}

void	ft_exec_child(t_shell *shell)
{
	if (cmd->type == EXEC)
	{
		if (execve(cmd->cmd, cmd->args, envp) == -1)
		{
			printf("minishell: %s: command not found\n", cmd->cmd);
			exit(1);
		}
	}
	else if (cmd->type == PIPE)
	{
		if (dup2(cmd->fd_in, 0) == -1)
		{
			printf("minishell: dup2 failed\n");
			exit(1);
		}
		if (dup2(cmd->fd_out, 1) == -1)
		{
			printf("minishell: dup2 failed\n");
			exit(1);
		}
		if (execve(cmd->cmd, cmd->args, envp) == -1)
		{
			printf("minishell: %s: command not found\n", cmd->cmd);
			exit(1);
		}
	}
}

void ft_free_all(t_shell *shell)
{
	if (shell->cmd != NULL)
		free_cmd(&shell->cmd);
	if (shell->line != NULL)
		free(shell->line);
}

void	exec_cmd(t_shell *shell)
{
	t_cmd	*cmd;
	char	**envp;

	cmd = shell->cmd;
	envp = shell->envp;
	while (cmd != NULL)
	{
		pid_t	pid;
		int		status;

		pid = ft_fork();
		if (pid == 0)
		{
			ft_exec_child(cmd, envp);
			ft_free_all(shell);
		}
		else
		{
			waitpid(pid, &status, 0);
			cmd = cmd->next;
		}
	}
}
