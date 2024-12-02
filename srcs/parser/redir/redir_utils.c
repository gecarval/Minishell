/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gecarval <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 09:11:09 by gecarval          #+#    #+#             */
/*   Updated: 2024/12/02 09:11:11 by gecarval         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

int	ft_strlen_meta(char *str)
{
	int	i;

	i = 0;
	while (str[i] != '\0' && str[i] != ' ' && str[i] != '\t' && str[i] != '>'
		&& str[i] != '<')
		i++;
	return (i);
}

char	*ft_strchrstr(char *str, char *to_find)
{
	int	i;
	int	j;

	i = 0;
	while (str[i] != '\0')
	{
		j = 0;
		while (str[i] != '\0' && to_find[j] != '\0')
			if (str[i] == to_find[j++])
				return (str + i);
		i++;
	}
	return (str + i);
}

char	*ft_strchr_dupfilename(char *line, int i)
{
	char	*filename;
	int		quote;
	int		j;

	quote = 0;
	if (line == NULL || line[i] == '\0')
		return (NULL);
	while (line[i] != '\0' && (line[i] == ' ' || line[i] == '\t'))
		line[i++] = ' ';
	if (line[i] == '\0' || line[i] == '\n' || line[i] == '>' || line[i] == '<'
		|| line[i] == '|')
		return (NULL);
	if (line[i] == '\'' || line[i] == '\"')
		quote = line[i];
	if (line[i] == '\'' || line[i] == '\"')
		line[i++] = ' ';
	j = i;
	if (quote > 0)
		while (line[j] != '\0' && line[j] != quote)
			j++;
	else
		while (line[j] != '\0' && line[j] != ' ' && line[j] != '\t'
			&& line[j] != '\n')
			j++;
	filename = (char *)ft_calloc((j - i + 1), sizeof(char));
	if (filename == NULL)
		return (NULL);
	j -= i;
	quote = 0;
	while (line[i] != '\0' && quote < j)
	{
		filename[quote++] = line[i];
		line[i++] = ' ';
	}
	if (line[i] == '\'' || line[i] == '\"')
		line[i] = ' ';
	return (filename);
}
