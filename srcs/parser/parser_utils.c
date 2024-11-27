/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gecarval <gecarval@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 09:18:11 by gecarval          #+#    #+#             */
/*   Updated: 2024/11/27 16:26:26 by gecarval         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// This function limits the buffer size of the line
// It returns NULL if the line is NULL
// It returns NULL and prints a error message if the line is too long
// Because some shells have a limit of characters in a command
char	*ft_limit_buffer(char *line)
{
	int	i;

	i = 0;
	if (!line)
		return (NULL);
	while (line[i] != '\0' && i < CMD_BUFFER)
		i++;
	if (i >= CMD_BUFFER)
	{
		printf("minishell: Argument list too long\n");
		return (line);
	}
	return (line);
}

// This function removes the quotes from the string
// It returns the string without quotes
char	*ft_remove_quotes(char *str, int len)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (str[i] != '\0' && i < len)
	{
		if (str[i] != '"' || str[i] != '\'')
			str[j++] = str[i];
		i++;
	}
	str[j] = '\0';
	return (str);
}

// This function checks if the line has a pipe
int	ft_is_pipe(char *line)
{
	int	i;

	i = -1;
	while (line[++i])
	{
		if (line[i] == '\"' || line[i] == '\'')
			i = ft_skiptochr(line, i + 1, line[i]);
		else if (line[i] == '|')
			return (1);
	}
	return (0);
}

// This function checks if the line has unclosed quotes
int	ft_check_unvalid(char *line)
{
	int	i;
	int	dquotes;
	int	squotes;

	i = 0;
	dquotes = 0;
	squotes = 0;
	while (line[i])
	{
		if (line[i] == '\"' && squotes % 2 == 0)
			dquotes++;
		if (line[i] == '\'' && dquotes % 2 == 0)
			squotes++;
		i++;
	}
	if (dquotes % 2 != 0 || squotes % 2 != 0)
		ft_putendl_fd("minishell: syntax error (unclosed quotes)", 2);
	if (dquotes % 2 != 0 || squotes % 2 != 0)
		return (1);
	return (0);
}
