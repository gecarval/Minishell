/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parser_split.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gecarval <gecarval@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 08:53:16 by gecarval          #+#    #+#             */
/*   Updated: 2024/11/06 10:58:40 by gecarval         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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

    if (!line || !delim)
        return (NULL);
    i = 0;
    new_line = ft_strdup(line);
    if (!new_line)
        return (NULL);
    while (new_line[i] != '\0')
    {
        if (new_line[i] == '"')
            while (new_line[++i] != '"' && new_line[i] != '\0')
                ;
        if (new_line[i] == *delim)
            new_line[i] = ';';
        i++;
    }
    matrix = ft_split(new_line, ';');
    free(new_line);
    return (matrix);
}
