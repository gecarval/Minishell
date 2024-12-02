/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_handler.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gecarval <gecarval@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 09:11:23 by gecarval          #+#    #+#             */
/*   Updated: 2024/12/02 09:48:18 by gecarval         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

void	ft_heredoc_handler(t_cmd *cmd)
{
	char	*line;
	int		fd[2];
	int		status;

	pipe(fd);
	while (1)
	{
		line = readline("heredoc> ");
		if (line == NULL)
			break ;
		if (ft_strcmp(line, cmd->fd.filename_in) == 0)
		{
			free(line);
			break ;
		}
		write(fd[1], line, ft_strlen(line));
		write(fd[1], "\n", 1);
		free(line);
	}
	close(fd[1]);
	waitpid(-1, &status, 0);
	dup2(fd[0], 0);
	close(fd[0]);
}

void	simulate_heredoc(const char *delimiter, const char *output_file)
{
	int		pipefd[2];
	pid_t	pid;
	int		fd;
	char	*line;

	// Create a pipe
	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		exit(EXIT_FAILURE);
	}
	// Fork a child process
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	if (pid == 0)
	{ // Child process
		// Close the write end of the pipe in the child process
		close(pipefd[1]);
		// Redirect the read end of the pipe to stdin
		dup2(pipefd[0], STDIN_FILENO);
		close(pipefd[0]);
		// Redirect stdin to an output file
		fd = open(output_file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
		if (fd == -1)
		{
			perror("open");
			exit(EXIT_FAILURE);
		}
		dup2(fd, STDOUT_FILENO);
		close(fd);
		// Execute a command (e.g., `cat` to display the input)
		execlp("cat", "cat", NULL);
		// If execlp fails
		perror("execlp");
		exit(EXIT_FAILURE);
	}
	else
	{ // Parent process
		// Close the read end of the pipe in the parent process
		close(pipefd[0]);
		// Use readline to accept input
		printf("Enter text (end with '%s'):\n", delimiter);
		while ((line = readline("> ")) != NULL)
		{
			// Check if the input matches the delimiter
			if (strcmp(line, delimiter) == 0)
			{
				free(line);
				break ;
			}
			// Write the input to the pipe
			//
			write(pipefd[1], line, strlen(line));
			write(pipefd[1], "\n", 1); // Add newline since readline strips it
			// Add the line to the history
			add_history(line);
			// Free the line buffer allocated by readline
			free(line);
		}
		// Close the write end of the pipe to signal EOF to the child
		close(pipefd[1]);
		// Wait for the child process to finish
		wait(NULL);
	}
}
