/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gecarval <gecarval@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 08:39:32 by gecarval          #+#    #+#             */
/*   Updated: 2024/12/06 16:34:52 by gecarval         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// verify the filename and if exits
// open the file, the fd and sets the redir type
void	ft_parse_redir_and_set_fd(char *line, t_fd *fds, t_shell *shell)
{
	int	i;

	i = -1;
	ft_init_fd(fds);
	while (line[++i] != '\0')
	{
		if (line[i] == '>')
		{
			ft_reset_fd_out(fds);
			ft_open_file(line, i, fds, shell);
			if (line[i + 1] == '>')
				i++;
		}
		else if (line[i] == '<')
		{
			ft_reset_fd_in(fds);
			ft_open_file(line, i, fds, shell);
      if (shell->heredoc_exitstatus == 130)
        return ;
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
	new->fd.filename_in = fds->filename_in;
	new->fd.filename_out = fds->filename_out;
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

int ft_free_mat(char **mat)
{
  int i;

  i = 0;
  if (mat == NULL)
    return (1);
  while (mat[i] != NULL)
    free(mat[i++]);
  free(mat);
  return (1);
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

	i = -1;
	if (ft_check_unvalid(shell->line) == 1)
		return ;
	cmds = ft_parser_split(shell->line, "|");
	while (cmds[++i] != NULL)
	{
		ft_parse_redir_and_set_fd(cmds[i], &fds, shell);
    if (shell->heredoc_exitstatus == 130 && ft_free_mat(cmds))
      return ;
		ft_expand_sign_matrix(&cmds[i], shell, 0);
		args = ft_parser_split(cmds[i], " \t");
		j = -1;
		while (args[++j] != NULL)
			ft_remove_quotes_logic(args[j], ft_strlen(args[j]));
		add_cmd(shell, args, &fds, ft_is_pipe(shell->line));
		ft_free_args(args);
		free(cmds[i]);
	}
	free(cmds);
}
