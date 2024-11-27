/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gecarval <gecarval@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 08:39:32 by gecarval          #+#    #+#             */
/*   Updated: 2024/11/21 16:39:22 by gecarval         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ft_strlen_meta(char *str)
{
	int	i;

	i = 0;
	while (str[i] != '\0' && str[i] != ' ' && str[i] != '\t' && str[i] != '>'
		&& str[i] != '<')
		i++;
	return (i);
}

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

char	*ft_strchrstr(char *str, char *to_find)
{
	int	i;
	int	j;

	i = 0;
	while (str[i] != '\0')
	{
		j = 0;
		while (str[i] != '\0' && to_find[j] != '\0')
			if (str[i] == to_find[j++])
				return (str + i);
		i++;
	}
	return (str + i);
}

char	*ft_strchr_dupfilename(char *strline, int i)
{
  char    *line;
  char	*filename;
  int		j;

    line = ft_strdup(strline);
    ft_remove_quotes_logic(line, ft_strlen(line));
    if (line[i] == '\0')
    {
      if (line != NULL)
        free(line);
      return (NULL);
    }
    while (line[i] != '\0' && (line[i] == ' ' || line[i] == '\t'))
      i++;
    if (line[i] == '\n' || line[i] == '\0' || line[i] == '>' || line[i] == '<'
        || line[i] == '|')
    {
      if (line != NULL)
        free(line);
      return (NULL);
    }
    j = i;
    while (line[j] != '\0' && (line[j] != ' ' || line[i] == '\t'))
      j++;
    filename = (char *)ft_calloc((j - i + 2), sizeof(char));
    if (filename == NULL)
    {
      if (line != NULL)
        free(line);
      return (NULL);
    }
    j = 0;
    while (line[i] != '\0' && line[i] != ' ' && line[i] != '\t'
        && line[i] != '\n')
    {
      filename[j++] = line[i];
      strline[i++] = ' ';
    }
    if (line != NULL)
      free(line);
    return (filename);
}

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

// verify the filename and if exits
// open the file, the fd and sets the redir type
void	ft_parse_redir_and_set_fd(char *line, t_fd *fds)
{
	int	i;

	i = -1;
	while (line[++i] != '\0')
	{
		if (line[i] == '>')
		{
			ft_reset_fd_out(fds);
			ft_open_file(line, i, fds);
			if (line[i + 1] == '>')
				i++;
		}
		else if (line[i] == '<')
		{
			ft_reset_fd_in(fds);
			ft_open_file(line, i, fds);
			if (line[i + 1] == '<')
				i++;
		}
		else if (line[i] == '\'' || line[i] == '\"')
			i = ft_skiptochr(line, i + 1, line[i]);
	}
}

// This function adds the arguments
// and the output to the command structure
// It counts the number of arguments
// It gets the flags using the ft_get_flags function
// It gets the output using the ft_get_cmdoutput function
void	add_args_and_output(t_cmd *new, char **args, t_fd *fds)
{
	int	i;

	i = 0;
	while (args[i] != NULL)
		i++;
	new->argc = i;
	new->args = ft_matdup(args);
	new->fd.fd_in = fds->fd_in;
	new->fd.fd_out = fds->fd_out;
	new->fd.filename_in = ft_strdup(fds->filename_in);
	new->fd.filename_out = ft_strdup(fds->filename_out);
}

// This function adds the command to the command structure List
// It allocates memory for the new command
// It adds the command to the command structure List
// If the command structure List is empty,
// it sets the new command as the head of the List
// If the command structure List is not empty,
// it sets the new command as the last command of the List
void	add_cmd(t_shell *shell, char **args, t_fd *fds, int is_pipe)
{
	t_cmd	*new;
	t_cmd	*last;

	new = (t_cmd *)malloc(sizeof(t_cmd));
	if (new == NULL)
		return ;
	new->next = NULL;
	new->cmd = ft_strdup(args[0]);
	if (is_pipe == 1)
		new->type = PIPE;
	else
		new->type = EXEC;
	add_args_and_output(new, args, fds);
	if (shell->cmd == NULL)
		shell->cmd = new;
	else
	{
		last = shell->cmd;
		while (last->next != NULL)
			last = last->next;
		last->next = new;
	}
}

// This function parses the line and adds the commands
// to the command structure List
// It splits the line by pipes and then by spaces using
// the ft_split function and ft_espur_str
// It then adds the command to the command structure List
// It frees the args and the new_line
void	parse_line(t_shell *shell)
{
	char	**cmds;
	char	**args;
	t_fd	fds;
	int		i;
	int		j;

	i = 0;
	if (ft_check_unvalid(shell->line) == 1)
		return ;
	cmds = ft_parser_split(shell->line, "|");
  ft_init_fd(&fds);
	while (cmds[i] != NULL)
	{
    ft_reset_fd(&fds);
		ft_expand_sign_matrix(&cmds[i], shell);
		ft_parse_redir_and_set_fd(cmds[i], &fds);
		args = ft_parser_split(cmds[i], " \t");
		j = -1;
		while (args[++j] != NULL)
			ft_remove_quotes_logic(args[j], ft_strlen(args[j]));
		add_cmd(shell, args, &fds, ft_is_pipe(shell->line));
		ft_free_args(args);
		free(cmds[i]);
		i++;
	}
	free(cmds);
}
