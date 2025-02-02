/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: badriano <badriano@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 11:16:11 by gecarval          #+#    #+#             */
/*   Updated: 2025/01/09 08:15:07 by gecarval         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_exit(t_shell *shell, int flag)
{
	int	fd;

	fd = 0;
	if (shell->cmd->argc == 2)
		fd = ft_exit_atol(shell->cmd->args[1]);
	if (shell->cmd->argc > 2)
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		return (1);
	}
	if (flag > 0)
		ft_putstr_fd("exit\n", 1);
	if (ft_lstsize_cmd(shell->cmd) > 1)
		return (fd);
	ft_free_all(shell, true, false);
	exit(fd);
	return (0);
}

void	ft_update_oldpwd_and_pwd_path(t_shell *shell)
{
	t_env	*oldpwd;
	t_env	*pwd;
	t_env	*tmp;

	tmp = shell->envp_list;
	pwd = NULL;
	oldpwd = NULL;
	while (tmp != NULL)
	{
		if (ft_strncmp(tmp->key, "OLDPWD", 6) == 0)
			oldpwd = tmp;
		if (ft_strncmp(tmp->key, "PWD", 3) == 0)
			pwd = tmp;
		tmp = tmp->next;
	}
	if (oldpwd != NULL && oldpwd->value != NULL)
		free(oldpwd->value);
	if (oldpwd != NULL && pwd != NULL)
		oldpwd->value = ft_strdup(pwd->value);
	else if (pwd == NULL && oldpwd != NULL)
		oldpwd->value = NULL;
	if (pwd != NULL && pwd->value != NULL)
		free(pwd->value);
	if (pwd != NULL)
		pwd->value = getcwd(NULL, 0);
}

int	ft_cd(t_cmd *cmd, t_shell *shell)
{
	int			chdrir_ret;
	struct stat	buf;

	if (cmd->argc == 1)
		return (0);
	else if (cmd->argc == 2)
	{
		chdrir_ret = chdir(cmd->args[1]);
		if (chdrir_ret == -1)
		{
			buf.st_mode = 0;
			ft_putstr_fd("minishell: cd: ", 2);
			ft_putstr_fd(cmd->args[1], 2);
			if (!S_ISDIR(buf.st_mode))
				ft_putstr_fd(": is not a directory\n", 2);
			else
				ft_putstr_fd(": No such file or directory\n", 2);
			return (1);
		}
		ft_update_oldpwd_and_pwd_path(shell);
		return (0);
	}
	ft_putstr_fd("minishell: cd: too many arguments\n", 2);
	return (1);
}

int	ft_pwd(void)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	ft_putstr_fd(cwd, STDOUT_FILENO);
	ft_putstr_fd("\n", STDOUT_FILENO);
	free(cwd);
	return (0);
}

int	ft_echo(t_cmd *cmd)
{
	int	i;
	int	flag;

	i = 1;
	flag = 0;
	while (cmd->args[i] && is_n_flag(cmd->args[i]))
	{
		flag = 1;
		i++;
	}
	while (cmd->args[i] != NULL)
	{
		ft_putstr_fd(cmd->args[i], STDOUT_FILENO);
		if (cmd->args[i + 1] != NULL)
			ft_putstr_fd(" ", STDOUT_FILENO);
		i++;
	}
	if (!flag)
		ft_putstr_fd("\n", STDOUT_FILENO);
	return (0);
}
