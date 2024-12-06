/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_handler.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gecarval <gecarval@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 09:11:23 by gecarval          #+#    #+#             */
/*   Updated: 2024/12/06 16:41:10 by gecarval         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

void	ft_heredoc_handler(t_fd *fds, t_shell *shell)
{
	char	*line;

	fds->fd_in = open(".heredoc_tmp", O_CREAT | O_WRONLY | O_TRUNC, 0644);
	while (1)
	{
		line = readline("heredoc> ");
		if (line == NULL)
			break ;
		if (ft_strncmp(line, fds->filename_in,
				ft_strlen(fds->filename_in)) == 0)
		{
			free(line);
			break ;
		}
		ft_expand_sign_matrix(&line, shell, 1);
		write(fds->fd_in, line, ft_strlen(line));
		write(fds->fd_in, "\n", 1);
		free(line);
	}
	close(fds->fd_in);
	fds->fd_in = open(".heredoc_tmp", O_RDONLY);
}
