/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gecarval <gecarval@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 08:40:26 by gecarval          #+#    #+#             */
/*   Updated: 2024/11/06 12:21:11 by gecarval         ###   ########.fr       */
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

void	ft_exec_on_child(t_shell *shell)
{
	char	*bin_route;

	bin_route = NULL;
	if (shell->cmd->cmd == NULL)
	{
		ft_free_all(shell);
		exit(0);
	}
	if (shell->cmd->cmd[0] != '/' && ft_strncmp(shell->cmd->cmd, "./", 2) != 0)
		bin_route = ft_strjoin("/bin/", shell->cmd->cmd);
	if (shell->cmd->cmd[0] != '/' && bin_route == NULL)
		bin_route = ft_strdup(shell->cmd->cmd);
	printf("bin_route: %s\n", bin_route);
	if (shell->cmd->type == EXEC)
	{
		ft_execve(bin_route, shell->cmd->args, shell->envp, shell);
	}
	else if (shell->cmd->type == PIPE)
	{
		if (dup2(shell->fd_in, 0) == -1)
		{
			printf("minishell: dup2 failed\n");
			free(bin_route);
			ft_free_all(shell);
			exit(1);
		}
		if (dup2(shell->fd_out, 1) == -1)
		{
			printf("minishell: dup2 failed\n");
			free(bin_route);
			ft_free_all(shell);
			exit(1);
		}
		ft_execve(bin_route, shell->cmd->args + 1, shell->envp, shell);
	}
	if (bin_route != NULL)
		free(bin_route);
}

// There is some builtins that are executed on the parent process
// This function checks if the command is a builtin
// If it is, it executes the builtin
// Because cd or pwd etc and etc need to execute on the parent process
// To changem the directory or print the working directory
// Since child processes are independent from the parent process
int	ft_exec_on_parent(t_cmd *cmd, t_shell *shell)
{
	int	workdone;

	workdone = 0;
	if (ft_strncmp(cmd->cmd, "exit", 5) == 0)
		workdone = ft_exit(shell);
	else if (ft_strncmp(cmd->cmd, "cd", 3) == 0)
		workdone = ft_cd(cmd);
	else if (ft_strncmp(cmd->cmd, "pwd", 4) == 0)
		workdone = ft_pwd(shell);
	else if (ft_strncmp(cmd->cmd, "export", 7) == 0)
		workdone = ft_export(cmd, shell);
	else if (ft_strncmp(cmd->cmd, "unset", 6) == 0)
		workdone = ft_unset(cmd, shell);
	else if (ft_strncmp(cmd->cmd, "env", 4) == 0)
		workdone = ft_env(shell);
	else if (ft_strncmp(cmd->cmd, "echo", 5) == 0)
		workdone = ft_echo(cmd, shell);
	return (workdone);
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
		if (ft_exec_on_parent(cmd, shell) == 1 && cmd->type == EXEC)
		{
			cmd = cmd->next;
			continue ;
		}
		pid = ft_fork(shell);
		if (pid == 0)
		{
			ft_exec_on_child(shell);
			ft_free_all(shell);
			exit(0);
		}
		else
		{
			//if (cmd->next == NULL && cmd->next->type == PIPE)
				waitpid(pid, &status, 0);
			cmd = cmd->next;
		}
	}
}
