/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gecarval <gecarval@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 09:11:09 by gecarval          #+#    #+#             */
/*   Updated: 2024/12/06 16:39:12 by gecarval         ###   ########.fr       */
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

int	ft_get_nameindex(char *line, int *quote, int *i, int *j)
{
	if (line == NULL || line[*i] == '\0')
		return (1);
	while (line[*i] != '\0' && (line[*i] == ' ' || line[*i] == '\t'))
		line[(*i)++] = ' ';
	if (line[*i] == '\0' || line[*i] == '\n' || line[*i] == '>'
		|| line[*i] == '<' || line[*i] == '|')
		return (1);
	if (line[*i] == '\'' || line[*i] == '\"')
		*quote = line[*i];
	if (line[*i] == '\'' || line[*i] == '\"')
		line[(*i)++] = ' ';
	*j = *i;
	if (*quote > 0)
		while (line[*j] != '\0' && line[*j] != *quote)
			(*j)++;
	else
		while (line[*j] != '\0' && line[*j] != ' ' && line[*j] != '\t'
			&& line[*j] != '\n')
			(*j)++;
	return (0);
}

char	*ft_strchr_dupfilename(char *line, int i, t_shell *shell, int expand)
{
	char	*filename;
	int		quote;
	int		j;

	j = 0;
	quote = 0;
	if (ft_get_nameindex(line, &quote, &i, &j) != 0)
		return (NULL);
	filename = (char *)ft_calloc((j - i + 5), sizeof(char));
	if (filename == NULL)
		return (NULL);
	j -= i;
	quote = 0;
	while (line[i] != '\0' && quote < j)
	{
		filename[quote++] = line[i];
		line[i++] = ' ';
	}
	if (expand == true && line[i] != '\'')
		ft_expand_sign_matrix(&filename, shell, 1);
	if (line[i] == '\'' || line[i] == '\"')
		line[i] = ' ';
	return (filename);
}
