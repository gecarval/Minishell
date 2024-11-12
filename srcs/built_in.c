/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gecarval <gecarval@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 11:16:11 by gecarval          #+#    #+#             */
/*   Updated: 2024/11/12 13:02:46 by gecarval         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_exit(t_shell *shell)
{
	ft_free_all(shell);
	ft_putendl_fd("exit", shell->fd_out);
	exit(0);
	return (1);
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
	if (oldpwd->value != NULL)
		free(oldpwd->value);
	oldpwd->value = ft_strdup(pwd->value);
	if (pwd->value != NULL)
		free(pwd->value);
	pwd->value = ft_strdup(cwd);
	free(cwd);
}

int	ft_cd(t_cmd *cmd, t_shell *shell)
{
	if (cmd->argc == 1)
		return (1);
	else if (cmd->argc == 2)
	{
		chdir(cmd->args[1]);
		ft_update_oldpwd_and_pwd_path(shell);
	}
	else
		printf("minishell: cd: too many arguments\n");
	return (1);
}

int	ft_pwd(t_shell *shell)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	ft_putstr_fd(cwd, shell->fd_out);
	ft_putstr_fd("\n", shell->fd_out);
	free(cwd);
	return (1);
}

int	ft_env(t_shell *shell)
{
	t_env	*tmp;

	tmp = shell->envp_list;
	while (tmp != NULL)
	{
		if (tmp->equal == 1)
		{
			ft_putstr_fd(tmp->key, shell->fd_out);
			ft_putstr_fd("=", shell->fd_out);
			ft_putstr_fd(tmp->value, shell->fd_out);
			ft_putstr_fd("\n", shell->fd_out);
		}
		tmp = tmp->next;
	}
	return (1);
}

int	ft_echo(t_cmd *cmd, t_shell *shell)
{
	int	flag;
	int	i;

	i = 1;
	flag = 0;
	while (cmd->args[i] != NULL)
	{
		if (i == 1 && ft_strncmp(cmd->args[i], "-n", 2) == 0)
		{
			i++;
			flag = 1;
			continue ;
		}
		else
			ft_putstr_fd(cmd->args[i], shell->fd_out);
		if (cmd->args[i + 1] != NULL)
			ft_putstr_fd(" ", shell->fd_out);
		i++;
	}
	if (flag == 0)
		ft_putstr_fd("\n", shell->fd_out);
	return (1);
}
