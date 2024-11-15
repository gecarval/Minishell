/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gecarval <gecarval@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 14:38:28 by gecarval          #+#    #+#             */
/*   Updated: 2024/11/15 09:01:01 by gecarval         ###   ########.fr       */
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

# ifndef PROMPT
#  define PROMPT "minishell$ "
# endif

# ifndef CMD_BUFFER
#  define CMD_BUFFER 131072
# endif

# ifndef EXEC
#  define EXEC 1
# endif

# ifndef REDIR
#  define REDIR 2
# endif

# ifndef PIPE
#  define PIPE 3
# endif

# ifndef LIST
#  define LIST 4
# endif

# ifndef BACK
#  define BACK 5
# endif

# ifndef HASH
#  define HASH 5
# endif

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

typedef struct	s_hashtable
{
	t_cmd	*cmd[HASH];
}	t_hashtable;

typedef struct s_env
{
	char			*key;
	int				equal;
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
	int				status;
	int				pipe_fd[2];
	int				fd_in;
	int				fd_out;
}					t_shell;

// MALLOC
char				**ft_matdup(char **mat);
char				*ft_limit_buffer(char *line);

// BUILTINS
int					ft_exit(t_shell *shell);
int					ft_cd(t_cmd *cmd, t_shell *shell);
int					ft_pwd(t_shell *shell);
int					ft_export(t_cmd *cmd, t_shell *shell);
int					ft_unset(t_cmd *cmd, t_shell *shell);
int					ft_env(t_shell *shell);
int					ft_echo(t_cmd *cmd, t_shell *shell);

// ENV_UTILS
t_env				*ft_get_envp_list(char **envp);
t_env				*ft_dupenv(t_env *env);
t_env				*ft_lstlast_envp(t_env *lst);
void				ft_sort_env(t_env *env);
void				ft_update_envp_matrix(t_shell *shell);
char				*ft_getenv(char *key, t_env **envp_list);
int					ft_lstsize_envp(t_env *lst);

// ENV
void				ft_export_new_key(char *arg, t_shell *shell);
int					ft_export_on_same_key(char *arg, t_shell *shell);
int					ft_invalid_key(char *str);

// PARSER_UTILS
void				add_args_and_output(t_cmd *new, char **args);
void				ft_handle_ispipe(t_cmd *new, int is_pipe);
char				*ft_limit_buffer(char *line);
char				*ft_remove_quotes(char *str, int len);
char				**ft_parser_split(char *line, char *delim, t_shell *shell);
int					ft_is_pipe(char *line);
int					ft_check_unvalid(char *line);

// EXPAND
void				ft_switch_flags(int *block_flag);
void				ft_expand_sign_matrix(char **matrix, t_shell *shell);
void				ft_deal_with_quotes(char **matrix, int i, int j,
						t_shell *shell);
void				ft_remove_quotes_logic(char *str, int len);
char				*ft_putstr_instr(char *str, char *insert_str,
						int insert_len, int insert_index);
char				*ft_strchr_validenv(const char *s);

// PARSER
void				add_args_and_output(t_cmd *new, char **args);
void				add_cmd(t_shell *shell, char **args, int is_pipe);
void				parse_line(t_shell *shell);

// FREE
void				free_cmd(t_cmd **cmd);
void				ft_free_args(char **args);
void				ft_free_all(t_shell *shell);
void				ft_free_envp_list(t_env *env);

// EXEC_UTILS
pid_t				ft_fork(t_shell *shell);

// EXEC
void				exec_cmd(t_shell *shell);

#endif

