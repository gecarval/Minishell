/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: badriano <badriano@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 16:10:21 by gecarval          #+#    #+#             */
/*   Updated: 2024/12/14 15:58:43 by badriano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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
		ft_free_all(shell, true, false);
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
		ft_free_all(shell, true, false);
		if (bin != NULL)
			free(bin);
		exit(127);
	}
}

void	ft_dup2_errors(char *filename, t_shell *shell)
{
	struct stat	buf;

	if (filename != NULL)
	{
		buf.st_mode = 0;
		stat(filename, &buf);
		ft_putstr_fd("minishell: ", 2);
		if (filename != NULL)
			ft_putstr_fd(filename, 2);
		if (S_ISDIR(buf.st_mode))
			ft_putstr_fd(": is a directory\n", 2);
		else if (access(filename, F_OK))
			ft_putstr_fd(": No such file or directory\n", 2);
		else
			ft_putstr_fd(": Permission denied\n", 2);
	}
	else
	{
		ft_putstr_fd("minishell: ", 2);
		if (filename != NULL)
			ft_putstr_fd(filename, 2);
		ft_putstr_fd(": No such file or directory\n", 2);
	}
	ft_free_all(shell, true, false);
	exit(1);
}

void	ft_dup2(int fd, int fd2, t_shell *shell, char *filename)
{
	if (fd == -1)
		ft_dup2_errors(filename, shell);
	if (dup2(fd, fd2) == -1)
	{
		ft_putstr_fd("minishell: dup2 failed\n", 2);
		ft_free_all(shell, true, false);
		exit(2);
	}
}

void	ft_signal_hand(int signum)
{
	(void)signum;
	write(1, "\n", 1);
}
