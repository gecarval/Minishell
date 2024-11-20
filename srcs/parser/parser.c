/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gecarval <gecarval@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 08:39:32 by gecarval          #+#    #+#             */
/*   Updated: 2024/11/20 16:40:11 by gecarval         ###   ########.fr       */
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

char	*ft_strchr_dupfilename(char *line, int i)
{
	char	*filename;
	int		j;

	ft_remove_quotes_logic(line, ft_strlen(line));
	if (line[i] == '\0')
		return (NULL);
	while (line[i] != '\0' && (line[i] == ' ' || line[i] == '\t'))
		i++;
	if (line[i] == '\n')
		return (NULL);
	j = i;
	while (line[j] != '\0' && line[j] != ' ')
		j++;
	filename = (char *)malloc(sizeof(char) * (j - i + 1));
	if (filename == NULL)
		return (NULL);
	j = 0;
	while (line[i] != '\0' && line[i] != ' ' && line[i] != '\t'
		&& line[i] != '\n')
		filename[j++] = line[i++];
	filename[j] = '\0';
	return (filename);
}

t_fd	ft_open_file(t_shell *shell, char *line, int i)
{
	t_fd	fds;

	if (line[i] == '>')
	{
		if (line[i + 1] == '>')
		{
			fds.filename_out = ft_strchr_dupfilename(line, i + 2);
			fds.fd_out = open(ft_strchrstr(line, ">>") + 2,
					O_CREAT | O_WRONLY | O_APPEND, 0644);
		}
		else
		{
			fds.filename_out = ft_strchr_dupfilename(line, i + 1);
			fds.fd_out = open(ft_strchrstr(line, ">") + 1,
					O_CREAT | O_WRONLY | O_TRUNC, 0644);
		}
	}
	else if (line[i] == '<')
	{
		fds.fd_in = open(ft_strstr(line, "<") + 1, O_RDONLY);
		fds.filename_in = ft_strchr_dupfilename(line, i + 1);
	}
	return (fds);
}

// This function removes the redirection and the filename
// It shifts the string to the left
// It sets the redirection to a space
void	ft_remove_redir_and_filename(char *line, int i)
{
	int	j;

	j = i;
	while (line[j] != '\0')
	{
		line[j] = line[j + 1];
		j++;
	}
	line[j] = ' ';
}

// verify the filename and if exits
// open the file, the fd and sets the redir type
t_fd	ft_parse_redir_and_set_fd(t_shell *shell, char *line)
{
	t_fd	fds;
	int		i;

	i = -1;
	fds.fd_in = STDIN_FILENO;
	fds.fd_out = STDOUT_FILENO;
	fds.filename_in = NULL;
	fds.filename_out = NULL;
	while (line[++i] != '\0')
	{
		if (line[i] == '>')
		{
			fds = ft_open_file(shell, line, i);
			ft_remove_redir_and_filename(line, i);
		}
		else if (line[i] == '<')
		{
			fds = ft_open_file(shell, line, i);
			ft_remove_redir_and_filename(line, i);
		}
		else if (line[i] == '\'' || line[i] == '\"')
			i = ft_skiptochr(line, i + 1, line[i]);
	}
	return (fds);
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
	new->fd = *fds;
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
	while (cmds[i] != NULL)
	{
		ft_expand_sign_matrix(&cmds[i], shell);
		fds = ft_parse_redir_and_set_fd(shell, cmds[i]);
		args = ft_parser_split(cmds[i], " \t\n");
		j = 0;
		while (args[++j] != NULL)
			ft_remove_quotes_logic(args[j], ft_strlen(args[j]));
		// ft_expand_sign_matrix(args, shell);
		add_cmd(shell, args, &fds, ft_is_pipe(shell->line));
		ft_free_args(args);
		free(cmds[i]);
		i++;
	}
	free(cmds);
}
