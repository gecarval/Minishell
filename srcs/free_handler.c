/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_handler.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gecarval <gecarval@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 09:22:00 by gecarval          #+#    #+#             */
/*   Updated: 2024/11/13 09:49:36 by gecarval         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// Just some free functions I think there is no need to explain
void	ft_free_all(t_shell *shell)
{
	if (shell->cmd != NULL)
		free_cmd(&shell->cmd);
	if (shell->envp != NULL)
		ft_free_args(shell->envp);
	if (shell->envp_list != NULL)
		ft_free_envp_list(shell->envp_list);
	if (shell->line != NULL)
		free(shell->line);
}

void	ft_free_envp_list(t_env *env)
{
	t_env	*tmp;

	while (env != NULL)
	{
		tmp = env->next;
		if (env->key != NULL)
			free(env->key);
		if (env->value != NULL)
			free(env->value);
		if (env != NULL)
			free(env);
		env = tmp;
	}
}

void	ft_free_args(char **args)
{
	int	i;

	i = 0;
	if (args == NULL)
		return ;
	while (args[i] != NULL)
		free(args[i++]);
	if (args != NULL)
		free(args);
	args = NULL;
}

void	free_cmd(t_cmd **cmd)
{
	t_cmd	*tmp;

	while (*cmd != NULL)
	{
		tmp = (*cmd)->next;
		if ((*cmd)->args != NULL)
			ft_free_args((*cmd)->args);
		if ((*cmd)->cmd != NULL)
			free((*cmd)->cmd);
		if (*cmd != NULL)
			free(*cmd);
		*cmd = tmp;
	}
}
