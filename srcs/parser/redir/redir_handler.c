/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_handler.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gecarval <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 09:11:15 by gecarval          #+#    #+#             */
/*   Updated: 2024/12/02 09:11:16 by gecarval         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

void	ft_open_file(char *line, int i, t_fd *fds)
{
	if (line[i] == '>')
	{
		line[i] = ' ';
		if (line[i + 1] == '>')
		{
			line[i + 1] = ' ';
			fds->filename_out = ft_strchr_dupfilename(line, i + 2);
			fds->fd_out = open(fds->filename_out, O_CREAT | O_WRONLY | O_APPEND,
					0644);
		}
		else
		{
			fds->filename_out = ft_strchr_dupfilename(line, i + 1);
			fds->fd_out = open(fds->filename_out, O_CREAT | O_WRONLY | O_TRUNC,
					0644);
		}
	}
	else if (line[i] == '<')
	{
		line[i] = ' ';
		if (line[i + 1] == '<')
		{
			line[i + 1] = ' ';
			fds->fd_heredoc = 1;
			fds->filename_in = ft_strchr_dupfilename(line, i + 2);
			fds->fd_in = open(fds->filename_in, O_RDONLY);
		}
		else
		{
			fds->fd_heredoc = 0;
			fds->filename_in = ft_strchr_dupfilename(line, i + 1);
			fds->fd_in = open(fds->filename_in, O_RDONLY);
		}
	}
}
