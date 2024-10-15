/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gecarval <gecarval@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 14:38:28 by gecarval          #+#    #+#             */
/*   Updated: 2024/10/15 14:31:28 by gecarval         ###   ########.fr       */
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
# define MAX_CMD 4096
# define EXEC 1
# define REDIR 2
# define PIPE 3
# define LIST 4
# define BACK 5

typedef struct s_cmd
{
	int				type;
}					t_cmd;

typedef struct s_execcmd
{
	int				type;
	char			**argv;
	char			**eargv;
}					t_execcmd;

typedef struct s_redircmd
{
	int				type;
	struct s_cmd	*cmd;
	char			*file;
	char			*efile;
	int				mode;
	int				fd;
}					t_redircmd;

typedef struct s_pipecmd
{
	int				type;
	struct s_cmd	*left;
	struct s_cmd	*right;
}					t_pipecmd;

typedef struct s_listcmd
{
	int				type;
	struct s_cmd	*left;
	struct s_cmd	*right;
}					t_listcmd;

typedef struct s_backcmd
{
	int				type;
	struct s_cmd	*cmd;
}					t_backcmd;

typedef struct s_minishell
{
	int				argc;
	char			**argv;
	char			**envp;
	t_cmd			*cmd;
	char			buf[MAX_CMD];
	int				fdout;
	int				fdint;
	int				pipe[2];
}					t_minishell;

#endif
