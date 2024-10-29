/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gecarval <gecarval@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 14:38:28 by gecarval          #+#    #+#             */
/*   Updated: 2024/10/29 13:24:55 by gecarval         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <dirent.h>
# include <fcntl.h>
# include <limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <stdio.h>
# include <sys/wait.h>
# include <termios.h>

# define PROMPT "minishell$ "
# define MAX_HISTORY 1024
# define CMD_BUFFER 1024
# define EXEC 1
# define REDIR 2
# define PIPE 3
# define LIST 4
# define BACK 5

typedef struct s_cmd
{
	char			*cmd;
	char			**args;
	char			*str_to_print;
	int				argc;
	int				type;
	int				fd_in;
	int				fd_out;
	struct s_cmd	*next;
}					t_cmd;

typedef struct s_shell
{
	t_cmd			*cmd;
	t_cmd			*new;
	t_cmd			*last;
	char			*line;
	char			**envp;
}					t_shell;

// BUILTINS

// FREE
void				free_cmd(t_cmd **cmd);
void				ft_free_args(char **args);

// EXEC
pid_t				ft_fork(void);
void				add_cmd(t_shell *shell, char **args, int is_pipe);
void				exec_cmd(t_shell *shell);

// PARSER_UTILS
void				add_args_and_output(t_cmd *new, char **args);
void				ft_handle_ispipe(t_cmd *new, int is_pipe);
void				ft_check_quotes(char *line);
char				*ft_limit_buffer(char *line);
char				*ft_espur_str(char *line);
int					ft_is_pipe(char *line);
// PARSER
void				parse_line(t_shell *shell);

#endif
