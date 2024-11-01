/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gecarval <gecarval@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 08:31:04 by gecarval          #+#    #+#             */
/*   Updated: 2024/11/01 08:15:02 by gecarval         ###   ########.fr       */
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

char *ft_strndup(const char *s, size_t n)
{
	char	*new;
	size_t	i;

	i = 0;
	if (s == NULL)
		return (NULL);
	new = (char *)malloc(sizeof(char) * (n + 1));
	if (new == NULL)
		return (NULL);
	while (i < n && s[i] != '\0')
	{
		new[i] = s[i];
		i++;
	}
	new[i] = '\0';
	return (new);
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
		tmp->value = ft_strdup(ft_strchr(envp[i], '=') + 1);
		tmp->next = env;
		env = tmp;
		i++;
	}
	return (env);
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
	shell.envp_list = ft_get_envp_list(envp);
	shell.envp = ft_matdup(envp);
	shell.line = NULL;
	shell.cmd = NULL;
	while (1)
	{
		shell.line = ft_limit_buffer(readline(PROMPT));
		if (!shell.line)
			break ;
		else if (*shell.line)
			add_history(shell.line);
		parse_line(&shell);
		ft_print_cmd(shell.cmd);
		exec_cmd(&shell);
		if (shell.cmd != NULL)
			free_cmd(&shell.cmd);
		if (shell.line != NULL)
			free(shell.line);
	}
	return (0);
}
