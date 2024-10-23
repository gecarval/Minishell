/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gecarval <gecarval@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 08:40:26 by gecarval          #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2024/10/23 09:39:53 by gecarval         ###   ########.fr       */
=======
/*   Updated: 2024/10/22 08:43:12 by gecarval         ###   ########.fr       */
>>>>>>> parent of d11d6e9 (added a lot of comments for intro to the code && fixed all leaks until now and all segfaults && finalized the commands parsing (if more is needed it will be added soon))
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

void	ft_exec_child(t_cmd *cmd, char **envp)
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

<<<<<<< HEAD
// This function executes the command
// It forks the process
// If the pid is 0, it executes the child process
// If the pid is not 0, it waits for the child process to finish
=======
void ft_free_all(t_shell *shell)
{
	if (shell->cmd != NULL)
		free_cmd(&shell->cmd);
	if (shell->line != NULL)
		free(shell->line);
}

>>>>>>> parent of d11d6e9 (added a lot of comments for intro to the code && fixed all leaks until now and all segfaults && finalized the commands parsing (if more is needed it will be added soon))
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
