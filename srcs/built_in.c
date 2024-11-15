/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gecarval <gecarval@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 11:16:11 by gecarval          #+#    #+#             */
/*   Updated: 2024/11/15 08:22:08 by gecarval         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_exit(t_shell *shell)
{
	ft_free_all(shell);
	ft_putendl_fd("exit", shell->fd_out);
	shell->status = 0;
	exit(0);
	return (0);
}

void	ft_update_oldpwd_and_pwd_path(t_shell *shell)
{
	t_env	*oldpwd;
	t_env	*pwd;
	t_env	*tmp;
	char	*cwd;

	tmp = shell->envp_list;
	while (tmp != NULL)
	{
		if (ft_strncmp(tmp->key, "OLDPWD", 6) == 0)
			oldpwd = tmp;
		if (ft_strncmp(tmp->key, "PWD", 3) == 0)
			pwd = tmp;
		tmp = tmp->next;
	}
	cwd = getcwd(NULL, 0);
	if (oldpwd != NULL && oldpwd->value != NULL)
		free(oldpwd->value);
	if (oldpwd != NULL && pwd != NULL)
		oldpwd->value = ft_strdup(pwd->value);
	if (pwd != NULL && pwd->value != NULL)
		free(pwd->value);
	if (pwd != NULL)
		pwd->value = ft_strdup(cwd);
	free(cwd);
}

int	ft_cd(t_cmd *cmd, t_shell *shell)
{
	int	chdrir_ret;

	if (cmd->argc == 1)
		return (0);
	else if (cmd->argc == 2)
	{
		chdrir_ret = chdir(cmd->args[1]);
		if (chdrir_ret == -1)
		{
			ft_putstr_fd("minishell: cd: ", 2);
			ft_putstr_fd(cmd->args[1], 2);
			ft_putstr_fd(": No such file or directory\n", 2);
			return (1);
		}
		ft_update_oldpwd_and_pwd_path(shell);
	}
	else
	{
		ft_putstr_fd("minishell: cd: too many arguments\n", 2);
		return (1);
	}
	return (0);
}

int	ft_pwd(t_shell *shell)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	ft_putstr_fd(cwd, shell->fd_out);
	ft_putstr_fd("\n", shell->fd_out);
	free(cwd);
	return (0);
}

int	ft_echo(t_cmd *cmd, t_shell *shell)
{
	int	flag;
	int	i;

	i = 0;
	flag = 0;
	while (cmd->args[++i] != NULL)
	{
		if (i == 1 && ft_strncmp(cmd->args[i], "-n", 2) == 0)
		{
			flag = i++;
			continue ;
		}
		else
			ft_putstr_fd(cmd->args[i], shell->fd_out);
		if (cmd->args[i + 1] != NULL)
			ft_putstr_fd(" ", shell->fd_out);
	}
	if (flag == 0)
		ft_putstr_fd("\n", shell->fd_out);
	return (0);
}
