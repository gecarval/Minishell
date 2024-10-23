/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gecarval <gecarval@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 09:18:11 by gecarval          #+#    #+#             */
/*   Updated: 2024/10/23 09:11:24 by gecarval         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// This function limits the buffer size of the line
// It returns NULL if the line is NULL
// It returns NULL and prints a error message if the line is too long
// Because some shells have a limit of characters in a command
char	*ft_limit_buffer(char *line)
{
    int		i;

    i = 0;
	if (!line)
		return (NULL);
    while (line[i] != '\0' && i < CMD_BUFFER)
        i++;
    if (i >= CMD_BUFFER)
    {
        free(line);
        printf("minishell: command too long\n");
        return (NULL);
    }
    return (line);
}

// This function removes the extra spaces from the line
char	*ft_espur_str(char *line)
{
	int		i;
	int		j;
	char	*new;

	if (!line)
		return (NULL);
	new = malloc(sizeof(char) * (ft_strlen(line) + 1));
	if (!new)
		return (NULL);
	i = 0;
	j = 0;
	while (line[i] == ' ')
		i++;
	while (line[i] != '\0')
	{
		if (line[i] == ' ' && line[i + 1] == ' ')
			while (line[i + 1] == ' ')
				i++;
		new[j] = line[i];
		i++;
		j++;
	}
	new[j] = '\0';
	return (new);
}

// This function checks if the line has a pipe
int	ft_is_pipe(char *line)
{
	int	i;

	i = 0;
	while (line[i])
	{
		if (line[i] == '|')
			return (1);
		i++;
	}
	return (0);
}

// This function checks if the line has unclosed quotes
void	ft_check_quotes(char *line)
{
	int	i;
	int	quotes;

	i = 0;
	quotes = 0;
	while (line[i])
	{
		if (line[i] == '\"' || line[i] == '\'')
			quotes++;
		i++;
	}
	if (quotes % 2 != 0)
	{
		printf("minishell: syntax error\n");
		exit(1);
	}
}
