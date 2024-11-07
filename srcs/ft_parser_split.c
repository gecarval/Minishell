/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parser_split.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gecarval <gecarval@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 08:53:16 by gecarval          #+#    #+#             */
/*   Updated: 2024/11/07 16:43:56 by gecarval         ###   ########.fr       */
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

void	ft_insert_str_in_str(char **str, char *insert, int index)
{
	char	*new_str;
	int		i;
	int		j;

	new_str = (char *)malloc(sizeof(char) * (ft_strlen(*str) + ft_strlen(insert)
				+ 1));
	i = 0;
	j = 0;
	while ((*str)[i] != '\0')
	{
		if (i == index)
		{
			while (insert[j] != '\0')
				new_str[i++] = insert[j++];
		}
		new_str[i] = (*str)[i];
		i++;
	}
	new_str[i] = '\0';
	free(*str);
	*str = new_str;
}

void	ft_expand_sign_matrix(char **matrix, t_shell *shell)
{
	int		i;
	int		j;
	int		expand_flag;
	char	*tmp;
    char    *tmp2;

	i = 0;
	expand_flag = 1;
	while (matrix[i] != NULL)
	{
		j = 0;
		while (matrix[i][j] != '\0')
		{
			if (matrix[i][j] == '\'' && expand_flag == 1)
				expand_flag = 0;
			else if (matrix[i][j] == '\'' && expand_flag == 0)
				expand_flag = 1;
			if (matrix[i][j] == '$' && expand_flag == 1)
			{
				tmp = ft_strndup(&matrix[i][j + 1], ft_strchr(&matrix[i][j + 1],
							' ') - &matrix[i][j + 1]);
                tmp2 = ft_strdup(ft_getenv(tmp, &shell->envp_list));
                free(tmp);
				ft_insert_str_in_str(&matrix[i], tmp2, j);
                free(tmp2);
			}
			j++;
		}
		i++;
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
	ft_expand_sign_matrix(matrix, shell);
	return (matrix);
}
