/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gecarval <gecarval@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 14:40:05 by gecarval          #+#    #+#             */
/*   Updated: 2024/11/12 12:51:17 by gecarval         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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
	while (arg[i] != '=' && arg[i] != '\0')
		i++;
	if (arg[i] == '\0')
		return (1);
	tmp = shell->envp_list;
	while (tmp != NULL)
	{
		if (ft_strncmp(tmp->key, ft_remove_quotes(arg, i), i - 1) == 0)
		{
			if (tmp->value != NULL)
				free(tmp->value);
			tmp->value = ft_strdup(ft_remove_quotes(&arg[i + 1],
						ft_strlen(&arg[i + 1])));
			return (1);
		}
		tmp = tmp->next;
	}
	return (0);
}

void	ft_export_new_key(char *arg, t_shell *shell)
{
	t_env	*new;
	t_env	*tmp;
	int		i;

	i = 0;
	while (arg[i] != '=' && arg[i] != '\0')
		i++;
	new = (t_env *)malloc(sizeof(t_env));
	if (new == NULL)
		return ;
	new->key = ft_strndup(arg, i);
	new->equal = 1;
	if (arg[i] == '\0')
		new->equal = 0;
	new->value = ft_strdup(arg + i + 1);
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
