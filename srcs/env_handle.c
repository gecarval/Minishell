/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_handle.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gecarval <gecarval@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 14:35:25 by gecarval          #+#    #+#             */
/*   Updated: 2024/11/07 15:02:49 by gecarval         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_env	*ft_get_envp_list(char **envp)
{
	t_env	*env;
	t_env	*tmp;
	int		i;

	i = 0;
	env = NULL;
	while (envp[i] != NULL)
	{
		tmp = (t_env *)malloc(sizeof(t_env));
		if (tmp == NULL)
			return (NULL);
		tmp->key = ft_strndup(envp[i], ft_strchr(envp[i], '=') - envp[i]);
		tmp->value = ft_strdup(ft_strchr(envp[i], '=') + 1);
		tmp->next = env;
		env = tmp;
		i++;
	}
	return (env);
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

int	ft_export(t_cmd *cmd, t_shell *shell)
{
	int	i;

	i = 0;
	if (cmd->argc == 1)
		ft_env(shell);
	while (cmd->args[++i] != NULL)
	{
		if (ft_invalid_key(cmd->args[i]) == 1)
		{
			printf("minishell: export: `%s': not a valid identifier\n",
				cmd->args[i]);
		}
		else if (ft_export_on_same_key(cmd->args[i], shell) == 0)
		{
			ft_export_new_key(cmd->args[i], shell);
		}
	}
	return (1);
}

int	ft_cmp_and_unset(t_env *tmp, t_env *prev, char *arg, t_shell *shell)
{
	if (ft_strncmp(tmp->key, arg, ft_strlen(arg)) == 0)
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
	return (0);
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
			if (ft_cmp_and_unset(tmp, prev, cmd->args[i], shell) == 1)
				break ;
			prev = tmp;
			tmp = tmp->next;
		}
		i++;
	}
	return (1);
}
