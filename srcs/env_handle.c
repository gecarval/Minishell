/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_handle.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gecarval <gecarval@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 14:35:25 by gecarval          #+#    #+#             */
/*   Updated: 2024/11/12 13:03:12 by gecarval         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void ft_add_env(t_env **env, t_env *src)
{
	t_env	*new;
	t_env	*tmp;

	new = (t_env *)malloc(sizeof(t_env));
	if (new == NULL)
		return ;
	new->key = ft_strdup(src->key);
	new->equal = src->equal;
	new->value = ft_strdup(src->value);
	new->next = NULL;
	if (*env == NULL)
		*env = new;
	else
	{
		tmp = *env;
		while (tmp->next != NULL)
			tmp = tmp->next;
		tmp->next = new;
	}
}

t_env	*ft_dupenv(t_env *env)
{
	t_env	*new;
	t_env	*tmp;

	new = NULL;
	tmp = env;
	while (tmp != NULL)
	{
		ft_add_env(&new, tmp);
		tmp = tmp->next;
	}
	return (new);
}

int	ft_export_print(t_shell *shell)
{
	t_env	*tmp;
	t_env	*new;

	new = ft_dupenv(shell->envp_list);
	ft_sort_env(new);
	tmp = new;
	while (tmp != NULL)
	{
		ft_putstr_fd("declare -x ", shell->fd_out);
		ft_putstr_fd(tmp->key, shell->fd_out);
		ft_putstr_fd("=\"", shell->fd_out);
		ft_putstr_fd(tmp->value, shell->fd_out);
		ft_putstr_fd("\"\n", shell->fd_out);
		tmp = tmp->next;
	}
	ft_free_envp_list(new);
	return (1);
}

int	ft_export(t_cmd *cmd, t_shell *shell)
{
	int	i;

	i = 0;
	if (cmd->argc == 1)
		return (ft_export_print(shell));
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
