/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parser_split.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gecarval <gecarval@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 08:53:16 by gecarval          #+#    #+#             */
/*   Updated: 2024/11/12 09:01:55 by gecarval         ###   ########.fr       */
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

void	switch_flags(int *block_flag)
{
	if (*block_flag == 0)
		*block_flag = 1;
	else if (*block_flag == 1)
		*block_flag = 0;
}

void	ft_remove_quotes_logic(char *str, int len)
{
	int	i;
	int	j;
	int	block_flag;

	i = 0;
	j = 0;
	block_flag = 0;
	while (str[i] != '\0' && i < len)
	{
		if (str[i] == '\"')
		{
			switch_flags(&block_flag);
			i++;
			continue;
		}
		if (str[i] == '\'' && block_flag == 0)
		{
			i++;
			continue;
		}
		str[j++] = str[i++];
	}
	while (str[j] != '\0' && j < len)
		str[j++] = '\0';
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

char	*ft_putstr_instr(char *str, char *insert, int insert_len, int index)
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
			k += insert_len;
		if (i == index)
			while (insert != NULL && insert[j] != '\0')
				new_str[i++] = insert[j++];
		new_str[i++] = str[k++];
	}
	new_str[i] = '\0';
	free(str);
	return (new_str);
}

void	ft_deal_with_quotes(char **matrix, int i, int j, t_shell *shell)
{
	static int	insert_flag = 0;
	static int	block_flag = 0;
	char		*tmp;
	char		*tmp2;

	if (matrix[i][j] == '\"' && insert_flag == 0)
		insert_flag = 1;
	else if (matrix[i][j] == '\"' && insert_flag == 1)
		insert_flag = 0;
	if (matrix[i][j] == '\'' && block_flag == 1 && insert_flag == 0)
		block_flag = 0;
	else if (matrix[i][j] == '\'' && block_flag == 0 && insert_flag == 0)
		block_flag = 1;
	if (matrix[i][j] == '$' && block_flag == 0)
	{
		tmp = ft_strndup(&matrix[i][j + 1], ft_strchr_validenv(&matrix[i][j
					+ 1]) - &matrix[i][j + 1]);
		tmp2 = ft_strdup(ft_getenv(tmp, &shell->envp_list));
		matrix[i] = ft_putstr_instr(matrix[i], tmp2, ft_strlen(tmp) + 1, j);
		free(tmp);
		free(tmp2);
	}
}

void	ft_expand_sign_matrix(char **matrix, t_shell *shell)
{
	int	i;
	int	j;

	i = -1;
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
