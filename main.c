/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gecarval <gecarval@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 08:31:04 by gecarval          #+#    #+#             */
/*   Updated: 2024/11/13 12:40:56 by gecarval         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/minishell.h"

// This function prints the command structure List for debugging purposes
void	ft_print_cmd(t_cmd *cmd)
{
	t_cmd	*tmp;
	int		i;

	tmp = cmd;
	while (tmp != NULL)
	{
		printf("cmd: %s\n", tmp->cmd);
		i = -1;
		if (tmp->args != NULL)
			while (tmp->args[++i] != NULL)
				printf("arg[%d]: %s\n", i, tmp->args[i]);
		printf("argc: %d\n", tmp->argc);
		printf("type: %d\n", tmp->type);
		tmp = tmp->next;
	}
}

void	ft_signal_handler(int signum)
{
	(void)signum;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
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

// This initializes the shell data struct
void	ft_init_shell(t_shell *shell, char **envp)
{
	shell->envp_list = ft_get_envp_list(envp);
	shell->envp = ft_matdup(envp);
	shell->line = NULL;
	shell->cmd = NULL;
	shell->pipe_fd[0] = 0;
	shell->pipe_fd[1] = 1;
	shell->fd_in = 0;
	shell->fd_out = 1;
}

// This start by initializing shell data struct
// Then it reads the line from the user
// If the line is empty, it breaks the loop
// If the line is not empty, it adds the line to the history
// Then it parses the line
// Then it prints the command structure List
// Then it executes the command
// Then it frees the command structure List
int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;

	(void)argc;
	(void)argv;
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	ft_init_shell(&shell, envp);
	while (1)
	{
		signal(SIGINT, ft_signal_handler);
		shell.line = ft_limit_buffer(readline(PROMPT));
		if (!shell.line)
			break ;
		else if (*shell.line)
			add_history(shell.line);
		parse_line(&shell);
		ft_print_cmd(shell.cmd);
		exec_cmd(&shell);
		free_cmd(&shell.cmd);
		if (shell.line != NULL)
			free(shell.line);
	}
	ft_free_all(&shell);
	return (0);
}
