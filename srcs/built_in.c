/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gecarval <gecarval@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 11:16:11 by gecarval          #+#    #+#             */
/*   Updated: 2024/11/07 15:16:04 by gecarval         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_exit(t_shell *shell)
{
	ft_free_all(shell);
	ft_putendl_fd("exit", shell->fd_out);
	exit(0);
	return (1);
}

int	ft_cd(t_cmd *cmd)
{
	if (cmd->argc == 1)
		return (1);
	else if (cmd->argc == 2)
		chdir(cmd->args[1]);
	else
		printf("minishell: cd: too many arguments\n");
	return (1);
}

int	ft_pwd(t_shell *shell)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	ft_putstr_fd(cwd, shell->fd_out);
	free(cwd);
	return (1);
}

int	ft_echo(t_cmd *cmd, t_shell *shell)
{
	char	*str;
	int		flag;
	int		i;

	i = 1;
	flag = 0;
	while (cmd->args[i] != NULL)
	{
		if (i == 1 && ft_strncmp(cmd->args[i], "-n", 2) == 0)
		{
			i++;
			flag = 1;
			continue ;
		}
		if (cmd->args[i][0] == '$')
		{
			str = ft_getenv(&cmd->args[i][1], &shell->envp_list);
			if (str == NULL)
				ft_putstr_fd("\n", shell->fd_out);
			ft_putstr_fd(str, shell->fd_out);
		}
		else
			ft_putstr_fd(cmd->args[i], shell->fd_out);
		if (cmd->args[i + 1] != NULL)
			ft_putstr_fd(" ", shell->fd_out);
		i++;
	}
	if (flag == 0)
		ft_putstr_fd("\n", shell->fd_out);
	return (1);
}
