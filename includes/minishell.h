/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gecarval <gecarval@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 14:38:28 by gecarval          #+#    #+#             */
/*   Updated: 2024/12/06 16:34:43 by gecarval         ###   ########.fr       */
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

// STRUCTS
typedef struct s_fd
{
	int				fd_in;
	int				fd_out;
	int				fd_heredoc;
	char			*filename_in;
	char			*filename_out;
}					t_fd;

typedef struct s_env
{
	char			*key;
	int				equal;
	char			*value;
	struct s_env	*next;
}					t_env;

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
	t_fd			fd;
	struct s_cmd	*next;
}					t_cmd;

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
}					t_shell;

// MALLOC
char				**ft_matdup(char **mat);
char				*ft_limit_buffer(char *line);

// FREE
void				free_cmd(t_cmd **cmd);
void				ft_free_args(char **args);
int					ft_free_all(t_shell *shell);
void				ft_free_envp_list(t_env *env);

// INIT
void				ft_signal_handler(int signum);
void				ft_init_shell(t_shell *shell, char **envp);

// UTILS
int					ft_exit_atol(char *str);
int					ft_lstsize_cmd(t_cmd *lst);
int					ft_chrcmpstr(char chr, char *str);
int					ft_skiptochr(char *line, int i, char chr);

// PARSER_UTILS
void				ft_handle_ispipe(t_cmd *new, int is_pipe);
char				*ft_limit_buffer(char *line);
char				**ft_parser_split(char *line, char *delim);
int					ft_is_pipe(char *line);
int					ft_check_unvalid(char *line);

// PARSER
void				add_args_and_output(t_cmd *new, char **args, t_fd *fds);
void				add_cmd(t_shell *shell, char **args, t_fd *fds,
						int is_pipe);
void				parse_line(t_shell *shell);

// REDIR_FD_UTILS
void				ft_init_fd(t_fd *fds);
void				ft_reset_fd_out(t_fd *fds);
void				ft_reset_fd_in(t_fd *fds);
void				ft_reset_fd(t_fd *fds);

// REDIR HEREDOC
void				ft_heredoc_handler(t_fd *fds, t_shell *shell);

// REDIR_UTILS
int					ft_strlen_meta(char *str);
char				*ft_strchrstr(char *str, char *to_find);
char				*ft_strchr_dupfilename(char *line, int i, t_shell *shell,
						int expand);

// REDIR
void				ft_open_file(char *line, int i, t_fd *fds, t_shell *shell);
void				ft_parse_redir_and_set_fd(char *line, t_fd *fds,
						t_shell *shell);

// EXPAND
void				ft_switch_flags(int *block_flag);
void				ft_expand_sign_matrix(char **matrix, t_shell *shell, int i);
void				ft_deal_with_quotes(char **matrix, int i, int j,
						t_shell *shell);
void				ft_remove_quotes_logic(char *str, int len);
char				*ft_putstr_instr(char *str, char *insert_str,
						int insert_len, int insert_index);
char				*ft_strchr_validenv(const char *s);

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

// BUILTINS
int					ft_exit(t_shell *shell);
int					ft_cd(t_cmd *cmd, t_shell *shell);
int					ft_pwd(void);
int					ft_export(t_cmd *cmd, t_shell *shell);
int					ft_unset(t_cmd *cmd, t_shell *shell);
int					ft_env(t_shell *shell);
int					ft_echo(t_cmd *cmd);

// EXEC_UTILS
void				ft_execve(char *bin, char **args, char **env,
						t_shell *shell);
void				ft_dup2(int fd, int fd2, t_shell *shell, char *bin_route);
void				ft_signal_hand(int signum);
pid_t				ft_fork(t_shell *shell);

// EXEC_BINS
void				ft_exec_on_path(t_shell *shell, t_cmd *cmd);
char				*ft_get_bin_based_on_path(char *bin_route, t_shell *shell,
						t_cmd *cmd);
int					ft_exec_on_builtin(t_cmd *cmd, t_shell *shell);
int					ft_exec_on_parent(t_cmd *cmd, t_shell *shell);

// EXEC
void				exec_cmd(t_shell *shell);

#endif
