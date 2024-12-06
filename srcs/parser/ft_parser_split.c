/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parser_split.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gecarval <gecarval@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 08:53:16 by gecarval          #+#    #+#             */
/*   Updated: 2024/12/06 08:36:48 by gecarval         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// ft_strdup but for matrix
char	**ft_matdup(char **mat)
{
	char	**new;
	int		i;

	i = 0;
	if (mat == NULL)
		return (NULL);
	while (mat[i] != NULL)
		i++;
	new = (char **)ft_calloc((i + 1), sizeof(char *));
	i = -1;
	while (mat[++i] != NULL)
		new[i] = ft_strdup(mat[i]);
	new[i] = NULL;
	return (new);
}

// This function splits the line by the delimiter
// It returns NULL if the line is NULL
// It returns NULL if the delimiter is NULL
// It returns NULL if the delimiter is not found in the line
// It allocates memory for the new line
// It copies the line until the delimiter
// And ignores delimiters if it is between quotes
// It returns a matrix
char	**ft_parser_split(char *line, char *delim)
{
	char	**matrix;
	char	*new_line;
	int		i;

	i = -1;
	new_line = ft_strdup(line);
	if (new_line == NULL)
		return (NULL);
	while (new_line[++i] != '\0')
	{
		if (new_line[i] == '\"')
			i = ft_skiptochr(new_line, ++i, '\"');
		else if (new_line[i] == '\'')
			i = ft_skiptochr(new_line, ++i, '\'');
		else if (ft_chrcmpstr(new_line[i], delim) == 0)
			new_line[i] = -32;
	}
	matrix = ft_split(new_line, -32);
	free(new_line);
	return (matrix);
}
