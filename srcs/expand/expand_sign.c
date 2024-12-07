/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_sign.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gecarval <gecarval@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 11:40:03 by gecarval          #+#    #+#             */
/*   Updated: 2024/12/06 16:34:02 by gecarval         ###   ########.fr       */
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
		while (matrix[i][j] != '\0')
		{
			ft_deal_with_quotes(matrix, i, j, shell);
			j++;
		}
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
		{
			ft_switch_flags(block_flag);
			i++;
			continue ;
		}
		if (str[i] == '\'' && block_flag[0] == 0)
		{
			ft_switch_flags(block_flag + 1);
			i++;
			continue ;
		}
		str[j++] = str[i++];
	}
	str[j] = '\0';
}

void	ft_sign_and_question_mark(char **matrix, int i, int j, t_shell *shell)
{
	char	*tmp;

	tmp = ft_itoa(shell->status);
	matrix[i] = ft_putstr_instr(matrix[i], tmp, 2, j);
	free(tmp);
}

void	ft_sign_and_env(char **matrix, int i, int j, t_shell *shell)
{
	char	*tmp;
	char	*tmp2;

	tmp = ft_strndup(&matrix[i][j + 1], ft_strchr_validenv(&matrix[i][j + 1])
			- &matrix[i][j + 1]);
	if (tmp == NULL)
		return ;
	tmp2 = ft_strdup(ft_getenv(tmp, &shell->envp_list));
	matrix[i] = ft_putstr_instr(matrix[i], tmp2, ft_strlen(tmp) + 1, j);
	free(tmp);
	free(tmp2);
}

// expand $ sign in the matrix with the environment variable using bash logic
void	ft_deal_with_quotes(char **matrix, int i, int j, t_shell *shell)
{
	static int	inside_quotes = 0;
	static int	block_flag = 0;

	if (matrix[i][j] == '\"')
		inside_quotes += 1;
	if (matrix[i][j] == '\'' && block_flag == 1 && inside_quotes % 2 != 1)
		block_flag = 0;
	else if (matrix[i][j] == '\'' && block_flag == 0 && inside_quotes % 2 != 1)
		block_flag = 1;
	if (matrix[i][j] == '$' && matrix[i][j + 1] == '?' && block_flag == 0)
		ft_sign_and_question_mark(matrix, i, j, shell);
	else if (matrix[i][j] == '$' && block_flag == 0)
		ft_sign_and_env(matrix, i, j, shell);
}
