/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gecarval <gecarval@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 14:40:05 by gecarval          #+#    #+#             */
/*   Updated: 2024/11/13 12:30:58 by gecarval         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*ft_getenv(char *key, t_env **envp_list)
{
	t_env	*tmp;

	tmp = *envp_list;
	if (key == NULL)
		return (NULL);
	while (tmp != NULL)
	{
		if (ft_strncmp(tmp->key, key, ft_strlen(tmp->key)) == 0)
			return (tmp->value);
		tmp = tmp->next;
	}
	return (NULL);
}

void	ft_sort_env(t_env *env)
{
	t_env	*tmp;
	char	*key;
	char	*value;

	tmp = env;
	while (tmp != NULL)
	{
		if (tmp->next != NULL && ft_strncmp(tmp->key, tmp->next->key,
				ft_strlen(tmp->key)) > 0)
		{
			key = tmp->key;
			value = tmp->value;
			tmp->key = tmp->next->key;
			tmp->value = tmp->next->value;
			tmp->next->key = key;
			tmp->next->value = value;
			tmp = env;
		}
		else
			tmp = tmp->next;
	}
}

int	ft_invalid_key(char *str)
{
	int	i;

	i = 1;
	if (str == NULL)
		return (1);
	if (ft_isalpha(str[0]) == 0 && str[0] != '_' && str[0] != '\"')
		return (1);
	while (str[i] != '=' && str[i] != '\0')
	{
		if (ft_isalnum(str[i]) == 0 && str[i] != '_' && str[i] != '\"')
			return (1);
		i++;
	}
	return (0);
}

int	ft_export_on_same_key(char *arg, t_shell *shell)
{
	t_env	*tmp;
	int		i;

	i = 0;
	while (arg[i] != '\0' && arg[i] != '=')
		i++;
	tmp = shell->envp_list;
	while (tmp != NULL)
	{
		if (ft_strncmp(tmp->key, arg, i - 1) == 0)
		{
			if (tmp->value != NULL && arg[i] != '\0')
			{
				free(tmp->value);
				tmp->value = NULL;
			}
			if (arg[i] == '=')
				tmp->equal = 1;
			if (arg[i] == '=' && arg[i + 1] != '\0')
				tmp->value = ft_strdup(arg + i + 1);
			return (1);
		}
		tmp = tmp->next;
	}
	return (0);
}

void	ft_export_new_key(char *arg, t_shell *shell)
{
	t_env	*new;
	t_env	*last;
	int		i;

	i = 0;
	while (arg[i] != '\0' && arg[i] != '=')
		i++;
	new = (t_env *)ft_calloc(1, sizeof(t_env));
	if (new == NULL)
		return ;
	new->key = ft_strndup(arg, i);
	new->equal = 1;
	if (arg[i] == '\0')
		new->equal = 0;
	if (arg[i] != '\0')
		new->value = ft_strdup(arg + i + 1);
	new->next = NULL;
	last = ft_lstlast_envp(shell->envp_list);
	if (shell->envp_list == NULL)
		shell->envp_list = new;
	else
		last->next = new;
}
