/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gecarval <gecarval@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 11:16:11 by gecarval          #+#    #+#             */
/*   Updated: 2024/11/06 12:53:56 by gecarval         ###   ########.fr       */
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

int	ft_cd(t_cmd *cmd)
{
	if (cmd->argc == 1)
		return (1);
	else if (cmd->argc == 2)
		chdir(cmd->args[1]);
	else
		printf("minishell: cd: too many arguments\n");
	return (1);
}

int	ft_pwd(t_shell *shell)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	ft_putstr_fd(cwd, shell->fd_out);
	free(cwd);
	return (1);
}

int	ft_remove_quotes(char *str)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (str[i] != '\0')
	{
		if (str[i] != '"')
			str[j++] = str[i];
		i++;
	}
	str[j] = '\0';
	return (1);
}

int	ft_export_on_same_key(t_cmd *cmd, t_shell *shell)
{
	int		i;
	t_env	*tmp;
	t_env	*prev;

	i = 1;
	while (cmd->args[i] != NULL)
	{
		tmp = shell->envp_list;
		prev = NULL;
		while (tmp != NULL)
		{
			if (ft_strncmp(tmp->key, cmd->args[i], ft_strchr(cmd->args[i], '=')
					- cmd->args[i]) == 0)
			{
				if (prev == NULL)
					shell->envp_list = tmp->next;
				else
					prev->next = tmp->next;
				if (tmp->key != NULL)
					free(tmp->key);
				if (tmp->value != NULL)
					free(tmp->value);
				free(tmp);
				return (1);
			}
			prev = tmp;
			tmp = tmp->next;
		}
		i++;
	}
	return (0);
}

int ft_invalid_key(char *str)
{
	int	i;

	i = 0;
	if (str == NULL)
		return (1);
	if (ft_isdigit(str[0]) == 1)
		return (1);
	while (str[i] != '=' && str[i] != '\0')
	{
		if (ft_isalnum(str[i]) == 0 || str[i] == '_')
			return (1);
		i++;
	}
	return (0);
}

int	ft_export(t_cmd *cmd, t_shell *shell)
{
	int		i;
	int		j;
	t_env	*new;
	t_env	*tmp;

	i = 1;
	if (ft_export_on_same_key(cmd, shell) == 1)
		return (1);
	while (cmd->args[i] != NULL)
	{
		if (ft_invalid_key(cmd->args[i]) == 1)
		{
			i++;
			continue ;
		}
		j = 0;
		while (cmd->args[i][j] != '=' && cmd->args[i][j] != '\0')
			j++;
		if (cmd->args[i][j] == '=')
		{
			new = (t_env *)malloc(sizeof(t_env));
			new->key = ft_strndup(cmd->args[i], j);
			if (cmd->args[i][j + 1] == '"')
				ft_remove_quotes(cmd->args[i]);
			new->value = ft_strdup(&cmd->args[i][j + 1]);
			new->next = NULL;
			if (shell->envp_list == NULL)
				shell->envp_list = new;
			else
			{
				tmp = shell->envp_list;
				while (tmp->next != NULL)
					tmp = tmp->next;
				tmp->next = new;
			}
		}
		else
			printf("minishell: export: `%s': not a valid identifier\n",
				cmd->args[i]);
		i++;
	}
	return (1);
}

int	ft_unset(t_cmd *cmd, t_shell *shell)
{
	int		i;
	t_env	*tmp;
	t_env	*prev;

	i = 1;
	while (cmd->args[i] != NULL)
	{
		tmp = shell->envp_list;
		prev = NULL;
		while (tmp != NULL)
		{
			if (ft_strncmp(tmp->key, cmd->args[i],
					ft_strlen(cmd->args[i])) == 0)
			{
				if (prev == NULL)
					shell->envp_list = tmp->next;
				else
					prev->next = tmp->next;
				if (tmp->key != NULL)
					free(tmp->key);
				if (tmp->value != NULL)
					free(tmp->value);
				free(tmp);
				break ;
			}
			prev = tmp;
			tmp = tmp->next;
		}
		i++;
	}
	return (1);
}

int	ft_env(t_shell *shell)
{
	t_env	*tmp;

	tmp = shell->envp_list;
	while (tmp != NULL)
	{
		if (tmp->value != NULL)
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

char	*ft_getenv(char *key, t_env **envp_list)
{
	t_env	*tmp;

	tmp = *envp_list;
	while (tmp != NULL)
	{
		if (ft_strncmp(tmp->key, key, ft_strlen(tmp->key)) == 0)
			return (tmp->value);
		tmp = tmp->next;
	}
	return (NULL);
}

int	ft_echo(t_cmd *cmd, t_shell *shell)
{
	char *str;
	int flag;
	int i;

	i = 1;
	flag = 0;
	while (cmd->args[i] != NULL)
	{
		if (i == 1 && ft_strncmp(cmd->args[i], "-n", 3) == 0)
		{
			i++;
			flag = 1;
			continue ;
		}
		if (cmd->args[i][0] == '$')
		{
			str = ft_getenv(&cmd->args[i][1], &shell->envp_list);
			if (str == NULL)
				ft_putstr_fd("\n", shell->fd_out);
			ft_putstr_fd(str, shell->fd_out);
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
