/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_handler.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gecarval <gecarval@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 09:11:15 by gecarval          #+#    #+#             */
/*   Updated: 2024/12/09 08:49:32 by gecarval         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

static void	ft_handle_output(char *line, int i, t_fd *fds, t_shell *shell)
{
	line[i] = ' ';
	if (line[i + 1] == '>')
	{
		line[i + 1] = ' ';
		fds->filename_out = ft_strchr_dupfilename(line, i + 2, shell, true);
		fds->fd_out = open(fds->filename_out, O_CREAT | O_WRONLY | O_APPEND,
				0644);
	}
	else
	{
		fds->filename_out = ft_strchr_dupfilename(line, i + 1, shell, true);
		fds->fd_out = open(fds->filename_out, O_CREAT | O_WRONLY | O_TRUNC,
				0644);
	}
}

static void	ft_handle_input(char *line, int i, t_fd *fds, t_shell *shell)
{
	line[i] = ' ';
	if (line[i + 1] == '<')
	{
		line[i + 1] = ' ';
		fds->fd_heredoc = 1;
		fds->filename_in = ft_strchr_dupfilename(line, i + 2, shell, false);
		ft_heredoc_handler(fds, shell);
		if (shell->heredoc_exitstatus == 130)
		{
			free(fds->filename_in);
			return ;
		}
	}
	else
	{
		fds->fd_heredoc = 0;
		fds->filename_in = ft_strchr_dupfilename(line, i + 1, shell, true);
		fds->fd_in = open(fds->filename_in, O_RDONLY);
	}
}

void	ft_open_file(char *line, int i, t_fd *fds, t_shell *shell)
{
	if (line[i] == '>')
		ft_handle_output(line, i, fds, shell);
	else if (line[i] == '<')
		ft_handle_input(line, i, fds, shell);
}
