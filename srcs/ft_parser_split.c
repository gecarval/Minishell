/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parser_split.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gecarval <gecarval@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 08:53:16 by gecarval          #+#    #+#             */
/*   Updated: 2024/11/12 12:03:40 by gecarval         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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
	new = (char **)malloc(sizeof(char *) * (i + 1));
	i = -1;
	while (mat[++i] != NULL)
		new[i] = ft_strdup(mat[i]);
	new[i] = NULL;
	return (new);
}

// This function expands the sign $ in the matrix
// It returns if the matrix is NULL
// It iterates over the matrix
// It iterates over the string in the matrix
// It calls the ft_deal_with_quotes function to expand the sign
// It calls the ft_remove_quotes_logic function in bash logic
void	ft_expand_sign_matrix(char **matrix, t_shell *shell)
{
	int	i;
	int	j;

	i = -1;
	if (matrix == NULL)
		return ;
	while (matrix[++i] != NULL)
	{
		j = -1;
		while (matrix[i][++j] != '\0')
			ft_deal_with_quotes(matrix, i, j, shell);
		ft_remove_quotes_logic(matrix[i], ft_strlen(matrix[i]));
	}
}

// This function splits the line by the delimiter
// It returns NULL if the line is NULL
// It returns NULL if the delimiter is NULL
// It returns NULL if the delimiter is not found in the line
// It allocates memory for the new line
// It copies the line until the delimiter
// And ignores delimiters if it is between quotes
// It returns a matrix
char	**ft_parser_split(char *line, char *delim, t_shell *shell)
{
	char	**matrix;
	char	*new_line;
	int		i;

	if (!line || !delim)
		return (NULL);
	i = -1;
	new_line = ft_strdup(line);
	if (!new_line)
		return (NULL);
	while (new_line[++i] != '\0')
	{
		if (new_line[i] == '"')
			while (new_line[++i] != '"' && new_line[i] != '\0')
				;
		if (new_line[i] == '\'')
			while (new_line[++i] != '\'' && new_line[i] != '\0')
				;
		if (new_line[i] == *delim)
			new_line[i] = ';';
	}
	matrix = ft_split(new_line, ';');
	free(new_line);
	ft_expand_sign_matrix(matrix, shell);
	return (matrix);
}
