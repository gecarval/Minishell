/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parser_split.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gecarval <gecarval@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 08:53:16 by gecarval          #+#    #+#             */
/*   Updated: 2024/11/08 10:43:14 by gecarval         ###   ########.fr       */
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

char	*ft_insert_str_in_str(char *str, char *insert, int insert_len,
		int index)
{
	char	*new_str;
	int		i;
	int		j;
	int		k;

	new_str = (char *)malloc(sizeof(char) * (ft_strlen(str) + ft_strlen(insert)
				+ 1));
	i = 0;
	j = 0;
	k = 0;
	while (str[k] != '\0')
	{
		if (i == index)
		{
			while (insert != NULL && insert[j] != '\0' && insert[0] != ' ')
				new_str[i++] = insert[j++];
			if (insert != NULL && insert[0] != ' ')
				k += insert_len;
		}
		new_str[i++] = str[k++];
	}
	new_str[i] = '\0';
	free(str);
	return (new_str);
}

char	*ft_strchr_validenv(const char *s)
{
	int	s_len;

	if (s == NULL)
		return ((char *)s);
	if (ft_isalpha((s[0]) == 0 && s[0] != '_') || s[0] == '\"')
		return ((char *)s);
	s_len = ft_strlen(s);
	while (s_len >= 0 && *s != '=')
	{
		if (*s == '\"' || *s == '\'' || (ft_isalnum(*s) == 0 && *s != '_'))
			return ((char *)s);
		s++;
		s_len--;
	}
	return ((char *)s);
}

void	ft_deal_with_quotes(char **matrix, int i, int j, t_shell *shell)
{
	static int	expand_flag[2] = {1, 0};
	char		*tmp;
	char		*tmp2;

	if (matrix[i][j] == '\"' && expand_flag[1] == 0)
		expand_flag[1] = 1;
	else if (matrix[i][j] == '\"' && expand_flag[1] == 1)
		expand_flag[1] = 0;
	if (matrix[i][j] == '\'' && expand_flag[0] == 1 && expand_flag[1] == 0)
		expand_flag[0] = 0;
	else if (matrix[i][j] == '\'' && expand_flag[0] == 0 && expand_flag[1] == 0)
		expand_flag[0] = 1;
	if (matrix[i][j] == '$' && expand_flag[0] == 1)
	{
		tmp = ft_strndup(&matrix[i][j + 1], ft_strchr_validenv(&matrix[i][j
					+ 1]) - &matrix[i][j + 1]);
		tmp2 = ft_strdup(ft_getenv(tmp, &shell->envp_list));
		if (tmp2 != NULL)
			matrix[i] = ft_insert_str_in_str(matrix[i], tmp2, ft_strlen(tmp)
					+ 1, j);
		else
			matrix[i] = ft_insert_str_in_str(matrix[i], " ", ft_strlen(tmp) + 1,
					j);
		free(tmp);
		free(tmp2);
	}
}

void	ft_expand_sign_matrix(char **matrix, t_shell *shell)
{
	int	i;
	int	j;

	i = 0;
	while (matrix[i] != NULL)
	{
		j = 0;
		while (matrix[i][j] != '\0')
		{
			ft_deal_with_quotes(matrix, i, j, shell);
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
		if (new_line[i] == '\'')
			while (new_line[++i] != '\'' && new_line[i] != '\0')
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
