/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_fd_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gecarval <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 09:11:19 by gecarval          #+#    #+#             */
/*   Updated: 2024/12/02 09:11:20 by gecarval         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

void	ft_init_fd(t_fd *fds)
{
	fds->fd_in = STDIN_FILENO;
	fds->fd_out = STDOUT_FILENO;
	fds->fd_heredoc = 0;
	fds->filename_in = NULL;
	fds->filename_out = NULL;
}

void	ft_reset_fd_out(t_fd *fds)
{
	if (fds->fd_out != STDOUT_FILENO)
	{
		close(fds->fd_out);
		fds->fd_out = STDOUT_FILENO;
	}
	if (fds->filename_out != NULL)
	{
		free(fds->filename_out);
		fds->filename_out = NULL;
	}
}

void	ft_reset_fd_in(t_fd *fds)
{
	if (fds->fd_heredoc == 1)
		fds->fd_heredoc = 0;
	if (fds->fd_in != STDIN_FILENO)
	{
		close(fds->fd_in);
		fds->fd_in = STDIN_FILENO;
	}
	if (fds->filename_in != NULL)
	{
		free(fds->filename_in);
		fds->filename_in = NULL;
	}
}

void	ft_reset_fd(t_fd *fds)
{
	if (fds->fd_in != STDIN_FILENO)
	{
		close(fds->fd_in);
		fds->fd_in = STDIN_FILENO;
	}
	if (fds->fd_out != STDOUT_FILENO)
	{
		close(fds->fd_out);
		fds->fd_out = STDOUT_FILENO;
	}
	if (fds->fd_heredoc == 1)
		fds->fd_heredoc = 0;
	if (fds->filename_in != NULL)
	{
		free(fds->filename_in);
		fds->filename_in = NULL;
	}
	if (fds->filename_out != NULL)
	{
		free(fds->filename_out);
		fds->filename_out = NULL;
	}
}
