/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_handle.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gecarval <gecarval@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 14:35:25 by gecarval          #+#    #+#             */
/*   Updated: 2024/12/11 13:16:45 by badriano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ft_export_print(t_shell *shell)
{
	t_env	*tmp;
	t_env	*new;

	new = ft_dupenv(shell->envp_list);
	ft_sort_env(new);
	tmp = new;
	while (tmp != NULL)
	{
		ft_putstr_fd("declare -x ", STDOUT_FILENO);
		ft_putstr_fd(tmp->key, STDOUT_FILENO);
		if (tmp->equal == 1)
		{
			ft_putstr_fd("=\"", STDOUT_FILENO);
			ft_putstr_fd(tmp->value, STDOUT_FILENO);
			ft_putstr_fd("\"\n", STDOUT_FILENO);
		}
		else
			ft_putstr_fd("\n", STDOUT_FILENO);
		tmp = tmp->next;
	}
	ft_free_envp_list(new);
	return (0);
}

int	ft_export(t_cmd *cmd, t_shell *shell)
{
	int	status;
	int	i;

	i = 0;
	status = 0;
	if (cmd->argc == 1)
		return (ft_export_print(shell));
	while (cmd->args[++i] != NULL)
	{
		if (ft_invalid_key(cmd->args[i]) == 1)
		{
			ft_putstr_fd("minishell: export: `", 2);
			ft_putstr_fd(cmd->args[i], 2);
			ft_putstr_fd("': not a valid identifier\n", 2);
			status = 1;
		}
		else if (ft_export_on_same_key(cmd->args[i], shell) == 0)
		{
			ft_export_new_key(cmd->args[i], shell);
		}
	}
	ft_update_envp_matrix(shell);
	return (status);
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
	ft_update_envp_matrix(shell);
	return (0);
}

int	ft_env(t_shell *shell)
{
	t_env	*tmp;

	tmp = shell->envp_list;
	while (tmp != NULL)
	{
		if (tmp->equal == 1)
		{
			ft_putstr_fd(tmp->key, STDOUT_FILENO);
			ft_putstr_fd("=", STDOUT_FILENO);
			ft_putstr_fd(tmp->value, STDOUT_FILENO);
			ft_putstr_fd("\n", STDOUT_FILENO);
		}
		tmp = tmp->next;
	}
	return (0);
}
