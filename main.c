/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gecarval <gecarval@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 14:49:45 by gecarval          #+#    #+#             */
/*   Updated: 2024/10/15 14:31:37 by gecarval         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/minishell.h"

// it just exits the program and prints the error message on stderr
void	panic(char *msg)
{
	perror(msg);
	exit(1);
}

// it creates a child process and returns the pid of the child process
// if the fork fails, it calls the panic function
pid_t	fork1(void)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		panic("fork");
	return (pid);
}

// it reads the command from the standard input
// latter readline will be used to read the command
int	getcmd(char *buf, int nbuf)
{
	if (isatty(fileno(stdin)))
		fprintf(stdout, "%s", PROMPT);
	memset(buf, 0, nbuf);
	fgets(buf, nbuf, stdin);
	if (buf[0] == 0)
		return (-1);
	return (0);
}

// it runs the command
// based on the type of the command, it will execute the command
void	runcmd(t_cmd *cmd)
{
	int		p[2];
	t_cmd	*left;
	t_cmd	*right;

	if (cmd == NULL)
		exit(0);
	switch (cmd->type)
	{
	case EXEC:
		execvp(((t_execcmd *)cmd)->argv[0], ((t_execcmd *)cmd)->argv);
		break ;
	case REDIR:
		break ;
	case PIPE:
		left = ((t_pipecmd *)cmd)->left;
		right = ((t_pipecmd *)cmd)->right;
		if (pipe(p) < 0)
			panic("pipe");
		if (fork1() == 0)
		{
			close(1);
			dup(p[1]);
			close(p[0]);
			close(p[1]);
			runcmd(left);
		}
		if (fork1() == 0)
		{
			close(0);
			dup(p[0]);
			close(p[0]);
			close(p[1]);
			runcmd(right);
		}
		close(p[0]);
		close(p[1]);
		wait(0);
		wait(0);
		break ;
	case LIST:
		left = ((t_listcmd *)cmd)->left;
		right = ((t_listcmd *)cmd)->right;
		if (fork1() == 0)
			runcmd(left);
		wait(0);
		runcmd(right);
		break ;
	case BACK:
		runcmd(((t_backcmd *)cmd)->cmd);
		break ;
	}
	exit(0);
}

// it parses the command
// it will return a pointer to the command
t_cmd	*parsecmd(char *s)
{
	t_cmd	*cmd;
		t_pipecmd *pcmd;
		t_listcmd *lcmd;
		t_execcmd *ecmd;
		char *argv[MAX_CMD];
		char *eargv[MAX_CMD];
		int i;

	cmd = malloc(sizeof(t_cmd));
	if (cmd == NULL)
		panic("malloc");
	ft_bzero(cmd, sizeof(t_cmd));
	if (strstr(s, "|"))
	{
		pcmd = malloc(sizeof(t_pipecmd));
		if (pcmd == NULL)
			panic("malloc");
		ft_bzero(pcmd, sizeof(t_pipecmd));
		pcmd->type = PIPE;
		pcmd->left = parsecmd(strtok(s, "|"));
		pcmd->right = parsecmd(strtok(NULL, "|"));
		cmd = (t_cmd *)pcmd;
	}
	else if (strstr(s, ";"))
	{
		lcmd = malloc(sizeof(t_listcmd));
		if (lcmd == NULL)
			panic("malloc");
		memset(lcmd, 0, sizeof(t_listcmd));
		lcmd->type = LIST;
		lcmd->left = parsecmd(strtok(s, ";"));
		lcmd->right = parsecmd(strtok(NULL, ";"));
		cmd = (t_cmd *)lcmd;
	}
	else
	{
		ecmd = malloc(sizeof(t_execcmd));
		if (ecmd == NULL)
			panic("malloc");
		memset(ecmd, 0, sizeof(t_execcmd));
		ecmd->type = EXEC;
		i = 0;
		argv[i] = strtok(s, " ");
		while (argv[i] != NULL)
		{
			eargv[i] = argv[i];
			argv[++i] = strtok(NULL, " ");
		}
		ecmd->argv = argv;
		ecmd->eargv = eargv;
		cmd = (t_cmd *)ecmd;
	}
	return (cmd);
}

// it displays a new prompt
void	display_new_prompt(int sig)
{
	(void)sig;
	fprintf(stdout, "\n%s", PROMPT);
}

int	main(int argc, char **argv, char **envp)
{
	t_minishell	shell;

	shell.argc = argc;
	shell.argv = argv;
	shell.envp = envp;
	// it sets the signal handler for the SIGINT and SIGQUIT signals
	signal(SIGINT, display_new_prompt);
	signal(SIGQUIT, SIG_IGN);
	// it reads the command from the standard input
	while (getcmd(shell.buf, sizeof(shell.buf)) >= 0)
	{
		if (strncmp(shell.buf, "cd", 2) == 0)
		{
			shell.buf[strlen(shell.buf) - 1] = 0;
			if (chdir(shell.buf + 3) < 0)
				perror("cd");
			continue ;
		}
		if (strncmp(shell.buf, "exit", 4) == 0)
			exit(0);
		else if (fork1() == 0)
			runcmd(parsecmd(shell.buf));
		wait(0);
	}
	return (0);
}
