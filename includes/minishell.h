/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gecarval <gecarval@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 14:38:28 by gecarval          #+#    #+#             */
/*   Updated: 2024/11/01 09:46:01 by gecarval         ###   ########.fr       */
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
# define CMD_BUFFER 1024
# define EXEC 1
# define REDIR 2
# define PIPE 3
# define LIST 4
# define BACK 5

// STRUCTS

// This structure is used to store the command structure in a List
// It contains the command
// The arguments and number of arguments
// The type of command (pipe or just exec)
// The input file descriptor and output file descriptor
// The string to print and the next command
typedef struct s_cmd
{
	char			*cmd;
	char			**args;
	int				argc;
	int				type;
	struct s_cmd	*next;
}					t_cmd;

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}					t_env;

// This structure is used to store the shell data
// It contains the command structure List
// The line read from the user
// The environment variables
typedef struct s_shell
{
	t_cmd			*cmd;
	t_env			*envp_list;
	char			*line;
	char			**envp;
	int				fd_in;
	int				fd_out;
}					t_shell;

// BUILTINS
int					ft_exit(t_shell *shell);
int					ft_cd(t_cmd *cmd);
int					ft_pwd(void);
int					ft_export(t_cmd *cmd, t_shell *shell);
int					ft_unset(t_cmd *cmd, t_shell *shell);
int					ft_env(t_shell *shell);

// MALLOC
char				**ft_matdup(char **mat);
char				*ft_limit_buffer(char *line);
char				*ft_espur_str(char *line);
char				*ft_strndup(const char *s, size_t n);
char				**ft_parser_split(char *line, char *delim);

// ENV
t_env				*ft_get_envp_list(char **envp);
char				**ft_getenv(t_shell *shell);

// FREE
void				free_cmd(t_cmd **cmd);
void				ft_free_args(char **args);
void				ft_free_all(t_shell *shell);

// EXEC
pid_t				ft_fork(t_shell *shell);
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
