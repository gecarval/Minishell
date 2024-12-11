/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gecarval <gecarval@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 11:16:11 by gecarval          #+#    #+#             */
/*   Updated: 2024/12/11 13:22:20 by badriano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_exit(t_shell *shell)
{
	int	fd;

	fd = 0;
	if (shell->cmd->argc == 2)
		fd = ft_exit_atol(shell->cmd->args[1]);
	if (shell->cmd->argc > 2)
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		return (2);
	}
	write(1, "exit\n", 5);
	if (ft_lstsize_cmd(shell->cmd) > 1)
		return (fd);
	ft_free_all(shell);
	exit(fd);
	return (0);
}

void	ft_update_oldpwd_and_pwd_path(t_shell *shell)
{
	t_env	*oldpwd;
	t_env	*pwd;
	t_env	*tmp;
	char	*cwd;

	tmp = shell->envp_list;
	oldpwd = NULL;
	pwd = NULL;
	while (tmp != NULL)
	{
		if (ft_strncmp(tmp->key, "OLDPWD", 6) == 0)
			oldpwd = tmp;
		if (ft_strncmp(tmp->key, "PWD", 3) == 0)
			pwd = tmp;
		tmp = tmp->next;
	}
	cwd = getcwd(NULL, 0);
	if (oldpwd != NULL && oldpwd->value != NULL)
		free(oldpwd->value);
	if (oldpwd != NULL && pwd != NULL)
		oldpwd->value = ft_strdup(pwd->value);
	if (pwd != NULL && pwd->value != NULL)
		free(pwd->value);
	if (pwd != NULL)
		pwd->value = ft_strdup(cwd);
	free(cwd);
}

int	ft_cd(t_cmd *cmd, t_shell *shell)
{
	int	chdrir_ret;

	if (cmd->argc == 1)
		return (0);
	else if (cmd->argc == 2)
	{
		chdrir_ret = chdir(cmd->args[1]);
		if (chdrir_ret == -1)
		{
			ft_putstr_fd("minishell: cd: ", 2);
			ft_putstr_fd(cmd->args[1], 2);
			ft_putstr_fd(": No such file or directory\n", 2);
			return (1);
		}
		ft_update_oldpwd_and_pwd_path(shell);
	}
	else
	{
		ft_putstr_fd("minishell: cd: too many arguments\n", 2);
		return (1);
	}
	return (0);
}

int	ft_pwd(void)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	ft_putstr_fd(cwd, STDOUT_FILENO);
	ft_putstr_fd("\n", STDOUT_FILENO);
	free(cwd);
	return (0);
}

// Helper function to check if a string is a valid -n flag
bool is_n_flag(const char *arg)
{
    int i = 1;

    // Check if the argument starts with '-'
    if (arg[0] != '-')
        return false;

    // Validate that all remaining characters are 'n'
    while (arg[i] != '\0')
    {
        if (arg[i] != 'n')
            return false;
        i++;
    }

    return (i > 1); // Ensure there's at least one 'n' after '-'
}


int ft_echo(t_cmd *cmd)
{
    int i = 1; // Start from the first argument after the command name
    int flag = 0;

    // Parse -n flags
    while (cmd->args[i] && is_n_flag(cmd->args[i]))
    {
        flag = 1; // Set flag if any -n is found
        i++;
    }

    // Print the remaining arguments
    while (cmd->args[i] != NULL)
    {
        ft_putstr_fd(cmd->args[i], STDOUT_FILENO);
        if (cmd->args[i + 1] != NULL)
            ft_putstr_fd(" ", STDOUT_FILENO);
        i++;
    }

    // Print newline only if no -n flag was found
    if (!flag)
        ft_putstr_fd("\n", STDOUT_FILENO);

    return 0;
}

