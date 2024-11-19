/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gecarval <gecarval@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 08:40:26 by gecarval          #+#    #+#             */
/*   Updated: 2024/11/19 16:37:44 by gecarval         ###   ########.fr       */
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
		ft_putstr_fd("minishell: fork failed\n", 2);
		ft_free_all(shell);
		exit(2);
	}
	return (pid);
}

void	ft_execve(char *bin, char **args, char **env, t_shell *shell)
{
	if (execve(bin, args, env) == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(bin, 2);
		ft_putstr_fd(": command not found\n", 2);
		ft_free_all(shell);
		if (bin != NULL)
			free(bin);
		exit(127);
	}
}

void	ft_dup2(int fd, int fd2, t_shell *shell, char *bin_route)
{
	if (dup2(fd, fd2) == -1)
	{
		ft_putstr_fd("minishell: dup2 failed\n", 2);
		ft_free_all(shell);
		if (bin_route != NULL)
			free(bin_route);
		exit(2);
	}
}

char	*ft_get_bin_based_on_path(char *bin_route, t_shell *shell, t_cmd *cmd)
{
	char	*tmp;
	char	**path;
	int		i;

	i = -1;
	tmp = ft_getenv("PATH", &shell->envp_list);
	path = ft_split(tmp, ':');
	while (path[++i] != NULL)
	{
		tmp = ft_strjoin(path[i], "/");
		bin_route = ft_strjoin(tmp, cmd->cmd);
		free(tmp);
		if (access(bin_route, F_OK) == 0)
		{
			ft_free_args(path);
			return (bin_route);
		}
		free(bin_route);
	}
	ft_free_args(path);
	return (ft_strdup(cmd->cmd));
}

void	ft_exec_on_path(t_shell *shell, t_cmd *cmd)
{
	char	*tmp;
	char	*bin_route;

	bin_route = NULL;
	if (access(cmd->cmd, F_OK) == 0)
		bin_route = ft_strdup(cmd->cmd);
	else
	{
		tmp = ft_getenv("PATH", &shell->envp_list);
		if (tmp == NULL)
		{
			ft_putstr_fd("minishell: \n", 2);
			ft_putstr_fd(cmd->cmd, 2);
			ft_putstr_fd(": command not found\n", 2);
			ft_free_all(shell);
			exit(127);
		}
		bin_route = ft_get_bin_based_on_path(bin_route, shell, cmd);
	}
	ft_execve(bin_route, cmd->args, shell->envp, shell);
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

	workdone = -1;
	if (ft_strncmp(cmd->cmd, "exit", 5) == 0)
		workdone = ft_exit(shell);
	else if (ft_strncmp(cmd->cmd, "cd", 3) == 0)
		workdone = ft_cd(cmd, shell);
	else if (ft_strncmp(cmd->cmd, "export", 7) == 0)
		workdone = ft_export(cmd, shell);
	else if (ft_strncmp(cmd->cmd, "unset", 6) == 0)
		workdone = ft_unset(cmd, shell);
	shell->status = workdone;
	return (workdone);
}

int	ft_exec_on_builtin(t_cmd *cmd, t_shell *shell)
{
	int	workdone;

	workdone = -1;
	if (ft_strncmp(cmd->cmd, "exit", 5) == 0)
		workdone = ft_exit(shell);
	else if (ft_strncmp(cmd->cmd, "cd", 3) == 0)
		workdone = ft_cd(cmd, shell);
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
	shell->status = workdone;
	return (workdone);
}

void	ft_signal_hand(int signum)
{
	(void)signum;
	write(1, "\n", 1);
}

int	ft_exec_if_pipe(t_shell *shell)
{
	t_cmd	*cmd;
	pid_t	pid;

	cmd = shell->cmd;
	while (cmd != NULL)
	{
		if (cmd->next != NULL)
			pipe(shell->pipe_fd);
		pid = ft_fork(shell);
		if (pid == 0)
		{
			if (cmd->next != NULL)
				ft_dup2(shell->pipe_fd[1], shell->fd_out, shell, NULL);
			close(shell->pipe_fd[0]);
			if (ft_exec_on_builtin(cmd, shell) >= 0)
			{
				close(shell->pipe_fd[1]);
				ft_free_all(shell);
				exit(shell->status);
			}
			ft_exec_on_path(shell, cmd);
			close(shell->pipe_fd[1]);
			ft_free_all(shell);
			exit(shell->status);
		}
		else
		{
			if (cmd->next != NULL)
				ft_dup2(shell->pipe_fd[0], shell->fd_in, shell, NULL);
			if (cmd->next != NULL)
				close(shell->pipe_fd[1]);
		}
		cmd = cmd->next;
	}
	waitpid(pid, &shell->status, 0);
	if (WIFEXITED(shell->status))
		shell->status = WEXITSTATUS(shell->status);
	else if (shell->status == 2)
		shell->status = (shell->status << 6) + 2;
	return (shell->status);
}

int	ft_normal_exec(t_cmd *cmd, t_shell *shell)
{
	if (ft_exec_on_builtin(cmd, shell) >= 0)
		exit(shell->status);
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
		signal(SIGQUIT, SIG_DFL);
	if (shell->cmd->type == PIPE)
			exit(ft_exec_if_pipe(shell));
		else
			exit(ft_normal_exec(shell->cmd, shell));
	}
	else
	{
		signal(SIGINT, ft_signal_hand);
		waitpid(pid, &shell->status, 0);
		if (WIFEXITED(shell->status))
			shell->status = WEXITSTATUS(shell->status);
		else if (shell->status == 2)
			shell->status = (shell->status << 6) + 2;
	}
}
