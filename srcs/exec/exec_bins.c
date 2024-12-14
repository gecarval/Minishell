/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_bins.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: badriano <badriano@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 16:06:42 by gecarval          #+#    #+#             */
/*   Updated: 2024/12/14 17:55:52 by gecarval         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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
	if (cmd->cmd[0] == '.' && access(cmd->cmd, F_OK) == 0)
		bin_route = ft_strdup(cmd->cmd);
	else
	{
		tmp = ft_getenv("PATH", &shell->envp_list);
		if (tmp == NULL)
		{
			ft_putstr_fd("minishell: \n", 2);
			ft_putstr_fd(cmd->cmd, 2);
			ft_putstr_fd(": command not found\n", 2);
			ft_free_all(shell, true);
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
	if (ft_strncmp(cmd->cmd, "exit", 4) == 0)
		workdone = ft_exit(shell, !ft_is_pipe(shell->line));
	else if (ft_strncmp(cmd->cmd, "cd", 2) == 0)
		workdone = ft_cd(cmd, shell);
	else if (ft_strncmp(cmd->cmd, "export", 6) == 0)
		workdone = ft_export(cmd, shell);
	else if (ft_strncmp(cmd->cmd, "unset", 5) == 0)
		workdone = ft_unset(cmd, shell);
	shell->status = ft_crtl_c(workdone);
	return (workdone);
}

int	ft_exec_on_builtin(t_cmd *cmd, t_shell *shell)
{
	int	workdone;

	workdone = -1;
	if (ft_strncmp(cmd->cmd, "exit", 4) == 0)
		workdone = ft_exit(shell, !ft_is_pipe(shell->line));
	else if (ft_strncmp(cmd->cmd, "cd", 2) == 0)
		workdone = ft_cd(cmd, shell);
	else if (ft_strncmp(cmd->cmd, "pwd", 3) == 0)
		workdone = ft_pwd();
	else if (ft_strncmp(cmd->cmd, "export", 6) == 0)
		workdone = ft_export(cmd, shell);
	else if (ft_strncmp(cmd->cmd, "unset", 5) == 0)
		workdone = ft_unset(cmd, shell);
	else if (ft_strncmp(cmd->cmd, "env", 3) == 0)
		workdone = ft_env(shell);
	else if (ft_strncmp(cmd->cmd, "echo", 4) == 0)
		workdone = ft_echo(cmd);
	shell->status = ft_crtl_c(workdone);
	return (workdone);
}
