/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   destructors.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gecarval <gecarval@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 19:27:13 by gecarval          #+#    #+#             */
/*   Updated: 2024/10/14 20:48:55 by gecarval         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	free_cmd(t_cmd *cmd)
{
	if (cmd != NULL)
		free(cmd);
}

void	free_execcmd(t_execcmd *cmd)
{
	if (cmd != NULL && cmd->argv != NULL)
		free(cmd->argv);
	if (cmd != NULL && cmd->eargv != NULL)
		free(cmd->eargv);
	if (cmd != NULL)
		free(cmd);
}

void	free_redircmd(t_redircmd *cmd)
{
	if (cmd != NULL && cmd->cmd != NULL)
		free_cmd(cmd->cmd);
	if (cmd != NULL && cmd->file != NULL)
		free(cmd->file);
	if (cmd != NULL && cmd->efile != NULL)
		free(cmd->efile);
	if (cmd != NULL)
		free(cmd);
}

void	free_pipecmd(t_pipecmd *cmd)
{
	if (cmd != NULL && cmd->left != NULL)
		free_cmd(cmd->left);
	if (cmd != NULL && cmd->right != NULL)
		free_cmd(cmd->right);
	if (cmd != NULL)
		free(cmd);
}

void	free_listcmd(t_listcmd *cmd)
{
	if (cmd != NULL && cmd->left != NULL)
		free_cmd(cmd->left);
	if (cmd != NULL && cmd->right != NULL)
		free_cmd(cmd->right);
	if (cmd != NULL)
		free(cmd);
}

void	free_backcmd(t_backcmd *cmd)
{
	if (cmd != NULL && cmd->cmd != NULL)
		free_cmd(cmd->cmd);
	if (cmd != NULL)
		free(cmd);
}
