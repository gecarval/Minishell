/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gecarval <gecarval@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 11:19:05 by gecarval          #+#    #+#             */
/*   Updated: 2024/12/15 03:17:40 by gecarval         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ft_add_env(t_env **env, t_env *src)
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

t_env	*ft_lstlast_envp(t_env *lst)
{
	t_env	*tmp;

	tmp = lst;
	if (tmp == NULL)
		return (NULL);
	while (tmp->next != NULL)
		tmp = tmp->next;
	return (tmp);
}

int	ft_lstsize_envp(t_env *lst)
{
	int		i;
	t_env	*tmp;

	i = 0;
	tmp = lst;
	while (tmp != NULL)
	{
		i++;
		tmp = tmp->next;
	}
	return (i);
}

void	ft_update_envp_matrix(t_shell *shell)
{
	t_env	*tmp;
	char	*tmp2;
	int		i;

	i = 0;
	tmp = shell->envp_list;
	ft_free_args(shell->envp);
	shell->envp = (char **)ft_calloc(ft_lstsize_envp(tmp) + 1, sizeof(char *));
	if (shell->envp == NULL)
		return ;
	while (tmp != NULL)
	{
		if (tmp->equal == 1)
		{
			if (shell->envp[i] != NULL)
				free(shell->envp[i]);
			tmp2 = ft_strjoin(tmp->key, "=");
			if (tmp->value != NULL)
				shell->envp[i] = ft_strjoin(tmp2, tmp->value);
			if (tmp2 != NULL)
				free(tmp2);
		}
		tmp = tmp->next;
		i++;
	}
}
