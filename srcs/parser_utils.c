/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gecarval <gecarval@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 09:18:11 by gecarval          #+#    #+#             */
/*   Updated: 2024/10/29 13:16:57 by gecarval         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*ft_limit_buffer(char *line)
{
    int		i;

    i = 0;
    while (line[i] && i < CMD_BUFFER)
        i++;
    if (i >= CMD_BUFFER)
    {
        free(line);
        printf("minishell: command too long\n");
        return (NULL);
    }
    return (line);
}

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
