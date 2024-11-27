/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_shell.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gecarval <gecarval@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 10:19:38 by gecarval          #+#    #+#             */
/*   Updated: 2024/11/27 16:27:50 by gecarval         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_default_fd(t_shell *shell)
{
	shell->fd_in = STDIN_FILENO;
	shell->fd_out = STDOUT_FILENO;
}

void	ft_signal_handler(int signum)
{
	(void)signum;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

// This initializes the shell data struct
void	ft_init_shell(t_shell *shell, char **envp)
{
	shell->envp_list = ft_get_envp_list(envp);
	shell->envp = ft_matdup(envp);
	shell->line = NULL;
	shell->cmd = NULL;
	shell->status = 0;
	shell->pipe_fd[0] = 0;
	shell->pipe_fd[1] = 1;
	shell->fd_in = STDIN_FILENO;
	shell->fd_out = STDOUT_FILENO;
}

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
		tmp->equal = 1;
		tmp->value = ft_strdup(ft_strchr(envp[i], '=') + 1);
		tmp->next = env;
		env = tmp;
		i++;
	}
	return (env);
}
