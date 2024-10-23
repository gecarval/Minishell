/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_handler.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gecarval <gecarval@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 09:22:00 by gecarval          #+#    #+#             */
/*   Updated: 2024/10/23 09:11:59 by gecarval         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// Just some free functions I think there is no need to explain
void ft_free_all(t_shell *shell)
{
	if (shell->cmd != NULL)
		free_cmd(&shell->cmd);
	if (shell->line != NULL)
		free(shell->line);
}

void	ft_free_args(char **args)
{
	int	i;

	i = 0;
	while (args[i] != NULL)
		free(args[i++]);
	free(args);
}

void	free_cmd(t_cmd **cmd)
{
	t_cmd	*tmp;

	while (*cmd != NULL)
	{
		tmp = (*cmd)->next;
		if ((*cmd)->args)
			ft_free_args((*cmd)->args);
		if ((*cmd)->cmd)
			free((*cmd)->cmd);
		if ((*cmd)->str_to_print)
			free((*cmd)->str_to_print);
		free(*cmd);
		*cmd = tmp;
	}
}
