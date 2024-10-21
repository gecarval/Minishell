/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gecarval <gecarval@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 08:40:26 by gecarval          #+#    #+#             */
/*   Updated: 2024/10/21 09:26:47 by gecarval         ###   ########.fr       */
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

void	exec_cmd(t_cmd *cmd, char **envp)
{
	pid_t	pid;
	int		status;

	pid = ft_fork();
	if (pid == 0)
	{
		if (cmd->fd_in != 0)
		{
			dup2(cmd->fd_in, 0);
			close(cmd->fd_in);
		}
		if (cmd->fd_out != 1)
		{
			dup2(cmd->fd_out, 1);
			close(cmd->fd_out);
		}
		if (execve(cmd->cmd, cmd->args, envp) == -1)
        {
            printf("minishell: %s: command not found\n", cmd->cmd);
            exit(1);
        }
	}
	else
		waitpid(pid, &status, 0);
}
