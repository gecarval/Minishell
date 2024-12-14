/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_sign.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gecarval <gecarval@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 11:40:03 by gecarval          #+#    #+#             */
/*   Updated: 2024/12/14 11:30:57 by gecarval         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// This function expands the sign $ in the matrix
// It returns if the matrix is NULL
// It iterates over the matrix
// It iterates over the string in the matrix
// It calls the ft_deal_with_quotes function to expand the sign
// It calls the ft_remove_quotes_logic function in bash logic
void	ft_expand_sign_matrix(char **matrix, t_shell *shell, int oneliner)
{
	int	i;
	int	j;

	i = 0;
	if (matrix == NULL || *matrix == NULL || **matrix == '\0')
		return ;
	while (matrix[i] != NULL)
	{
		j = 0;
		while (ft_strlen(matrix[i]) > (size_t)j && matrix[i][j] != '\0')
		{
			j += ft_deal_with_quotes(matrix, i, j, shell);
		}
		ft_deal_with_quotes(matrix, i, -69, shell);
		if (oneliner == 1)
			break ;
		i++;
	}
}

// remove the quotes from the string in bash logic
void	ft_remove_quotes_logic(char *str, int len)
{
	int	i;
	int	j;
	int	block_flag[2];

	i = 0;
	j = 0;
	block_flag[0] = 0;
	block_flag[1] = 0;
	while (str != NULL && str[i] != '\0' && i < len)
	{
		if (str[i] == '\"' && block_flag[1] == 0)
			ft_switch_flags(block_flag);
		else if (str[i] == '\'' && block_flag[0] == 0)
			ft_switch_flags(block_flag + 1);
		else
		{
			if (str[i] == ('\'' * -1) || str[i] == ('\"' * -1))
				str[i] *= -1;
			str[j++] = str[i];
		}
		i++;
	}
	str[j] = '\0';
}

int	ft_sign_and_question_mark(char **matrix, int i, int j, t_shell *shell)
{
	char	*tmp;

	tmp = ft_itoa(shell->status);
	matrix[i] = ft_putstr_instr(matrix[i], tmp, 2, j);
	free(tmp);
	return (1);
}

int	ft_sign_and_env(char **matrix, int i, int j, t_shell *shell)
{
	int		expanded;
	int		k;
	char	*tmp;
	char	*tmp2;

	tmp = ft_strndup(&matrix[i][j + 1], ft_strchr_validenv(&matrix[i][j + 1])
			- &matrix[i][j + 1]);
	tmp2 = ft_strdup(ft_getenv(tmp, &shell->envp_list));
	expanded = 0;
	if (tmp2 != NULL)
		expanded = 1;
	k = -1;
	while (tmp2 != NULL && tmp2[++k] != '\0')
		if (tmp2[k] == '\'' || tmp2[k] == '\"')
			tmp2[k] *= -1;
	matrix[i] = ft_putstr_instr(matrix[i], tmp2, ft_strlen(tmp) + 1, j);
	free(tmp);
	free(tmp2);
	return (expanded);
}

// expand $ sign in the matrix with the environment variable using bash logic
int	ft_deal_with_quotes(char **matrix, int i, int j, t_shell *shell)
{
	int			expanded;
	static int	inside_quotes = 0;
	static int	block_flag = 0;

	expanded = 1;
	if (ft_strlen(matrix[i]) < (size_t)j)
		return (expanded);
	ft_quotes_block_toggle(matrix[i][j], &block_flag, &inside_quotes);
	if (ft_is_end_of_var(matrix[i], block_flag, inside_quotes, j) == true)
		;
	else if (matrix[i][j] == '$' && (matrix[i][j + 1] == '\'' || matrix[i][j
			+ 1] == '\"') && (block_flag == 0 && inside_quotes % 2 != 1))
	{
		expanded = 0;
		matrix[i] = ft_putstr_instr(matrix[i], NULL, 1, j);
	}
	else if (matrix[i][j] == '$' && matrix[i][j + 1] == '?' && block_flag == 0)
		expanded = ft_sign_and_question_mark(matrix, i, j, shell);
	else if (matrix[i][j] == '$' && block_flag == 0)
		expanded = ft_sign_and_env(matrix, i, j, shell);
	if (j == -69)
		inside_quotes = 0;
	if (j == -69)
		block_flag = 0;
	return (expanded);
}
