/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_sign.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gecarval <gecarval@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 11:40:03 by gecarval          #+#    #+#             */
/*   Updated: 2024/11/13 11:29:47 by gecarval         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	switch_flags(int *block_flag)
{
	if (*block_flag == 0)
		*block_flag = 1;
	else if (*block_flag == 1)
		*block_flag = 0;
}

// remove the quotes from the string in bash logic
void	ft_remove_quotes_logic(char *str, int len)
{
	int	i;
	int	j;
	int	block_flag;

	i = 0;
	j = 0;
	block_flag = 0;
	while (str != NULL && str[i] != '\0' && i < len)
	{
		if (str[i] == '\"')
		{
			switch_flags(&block_flag);
			i++;
			continue ;
		}
		if (str[i] == '\'' && block_flag == 0)
		{
			i++;
			continue ;
		}
		str[j++] = str[i++];
	}
	while (str[j] != '\0' && j < len)
		str[j++] = '\0';
}

// find the first invalid character in the string for the environment variable
// return the pointer to the first invalid character
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

	i = 0;
	j = 0;
	k = 0;
	new_str = (char *)ft_calloc(ft_strlen(str) + ft_strlen(insert) + 1,
			sizeof(char));
	while (str[k] != '\0')
	{
		if (i == index)
			k += insert_len;
		if (i == index)
			while (insert != NULL && insert[j] != '\0')
				new_str[i++] = insert[j++];
		new_str[i++] = str[k++];
	}
	free(str);
	return (new_str);
}

// expand $ sign in the matrix with the environment variable using bash logic
void	ft_deal_with_quotes(char **matrix, int i, int j, t_shell *shell)
{
	static int	outside_quotes = 0;
	static int	block_flag = 0;
	char		*tmp;
	char		*tmp2;

	if (matrix[i][j] == '\"')
		outside_quotes += 1;
	if (matrix[i][j] == '\'' && block_flag == 1 && outside_quotes % 2 == 0)
		block_flag = 0;
	else if (matrix[i][j] == '\'' && block_flag == 0 && outside_quotes % 2 == 0)
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
