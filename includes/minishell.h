/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gecarval <gecarval@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 14:38:28 by gecarval          #+#    #+#             */
/*   Updated: 2024/11/12 12:34:25 by gecarval         ###   ########.fr       */
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
# define CMD_BUFFER 131072
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

// ENV
t_env				*ft_get_envp_list(char **envp);
void				ft_sort_env(t_env *env);
void				ft_export_new_key(char *arg, t_shell *shell);
void				ft_sort_env(t_env *env);
char				*ft_getenv(char *key, t_env **envp_list);
int					ft_invalid_key(char *str);
int					ft_export_on_same_key(char *arg, t_shell *shell);

// PARSER_UTILS
void				add_args_and_output(t_cmd *new, char **args);
void				ft_handle_ispipe(t_cmd *new, int is_pipe);
char				*ft_limit_buffer(char *line);
char				*ft_remove_quotes(char *str, int len);
char				**ft_parser_split(char *line, char *delim, t_shell *shell);
int					ft_is_pipe(char *line);
int					ft_check_unvalid(char *line);

// EXPAND
void				ft_expand_sign_matrix(char **matrix, t_shell *shell);
void				ft_deal_with_quotes(char **matrix, int i, int j, t_shell *shell);
void				ft_remove_quotes_logic(char *str, int len);
char				*ft_putstr_instr(char *str, char *insert, int insert_len, int index);
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
