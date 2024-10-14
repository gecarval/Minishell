/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   contructors.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gecarval <gecarval@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 19:14:15 by gecarval          #+#    #+#             */
/*   Updated: 2024/10/14 20:29:37 by gecarval         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_cmd	*new_cmd(int type)
{
	t_cmd	*new;

	new = malloc(sizeof(t_cmd));
	if (!new)
		return (NULL);
	ft_bzero(new, sizeof(t_cmd));
	new->type = type;
	return (new);
}

t_cmd	*new_execcmd(char **argv, char **eargv)
{
	t_execcmd	*new;

	new = malloc(sizeof(t_cmd));
	if (!new)
		return (NULL);
	ft_bzero(new, sizeof(t_cmd));
	new->type = EXEC;
	new->argv = argv;
	new->eargv = eargv;
	return ((t_cmd *)new);
}

t_cmd	*new_redircmd(t_cmd *cmd, char *file, char *efile, int fd)
{
	t_redircmd	*new;

	new = malloc(sizeof(t_cmd));
	if (!new)
		return (NULL);
	ft_bzero(new, sizeof(t_cmd));
	new->type = REDIR;
	new->cmd = cmd;
	new->file = file;
	new->efile = efile;
	new->fd = fd;
	return ((t_cmd *)new);
}

t_cmd	*new_pipecmd(t_cmd *cmd1, t_cmd *cmd2)
{
	t_pipecmd	*new;

	new = malloc(sizeof(t_cmd));
	if (!new)
		return (NULL);
	ft_bzero(new, sizeof(t_cmd));
	new->type = PIPE;
	new->left = cmd1;
	new->right = cmd2;
	return ((t_cmd *)new);
}

t_cmd	*new_listcmd(t_cmd *cmd1, t_cmd *cmd2)
{
	t_listcmd	*new;

	new = malloc(sizeof(t_cmd));
	if (!new)
		return (NULL);
	ft_bzero(new, sizeof(t_cmd));
	new->type = LIST;
	new->left = cmd1;
	new->right = cmd2;
	return ((t_cmd *)new);
}

t_cmd	*new_backcmd(t_cmd *cmd)
{
	t_backcmd	*new;

	new = malloc(sizeof(t_backcmd));
	if (!new)
		return (NULL);
	ft_bzero(new, sizeof(t_cmd));
	new->type = BACK;
	new->cmd = cmd;
	return ((t_cmd *)new);
}
