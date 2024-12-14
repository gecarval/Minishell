/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gecarval <gecarval@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 09:18:11 by gecarval          #+#    #+#             */
/*   Updated: 2024/12/14 10:56:02 by gecarval         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*ft_limit_buffer(char *line)
{
	int	i;

	i = 0;
	if (!line)
		return (NULL);
	while (line[i] != '\0' && i < CMD_BUFFER)
		i++;
	if (i >= CMD_BUFFER)
	{
		printf("minishell: Argument list too long\n");
		return (line);
	}
	return (line);
}

int	ft_is_pipe(char *line)
{
	int	i;

	i = -1;
	while (line[++i])
	{
		if (line[i] == '\"' || line[i] == '\'')
			i = ft_skiptochr(line, i + 1, line[i]);
		else if (line[i] == '|')
			return (1);
	}
	return (0);
}

int	ft_check_unexpected_condtitions(char *line, int *i, int quotes, int *token)
{
	if (line[*i] == '|' && quotes == -1)
	{
		if (handle_pipe(token))
			return (1);
	}
	else if (line[*i] == '>' && quotes == -1)
	{
		if (handle_redirection(token, line, i))
			return (1);
	}
	else if (line[*i] == '<' && quotes == -1)
	{
		if (handle_less_than(token, line, i))
			return (1);
	}
	else if (quotes == -1)
	{
		if (handle_other_chars(line[*i], token))
			return (1);
	}
	return (0);
}

int	ft_check_unexpected(char *line)
{
	int	quotes;
	int	token;
	int	i;

	quotes = -1;
	token = -1;
	i = 0;
	while (line[i] != '\0')
	{
		if (line[i] == '\"' || line[i] == '\'')
		{
			token = -1;
			quotes = toggle_quotes(quotes);
		}
		else if (ft_check_unexpected_condtitions(line, &i, quotes, &token) == 1)
		{
			ft_crtl_c(2);
			return (1);
		}
		i++;
	}
	if (token == 1)
		return (1);
	return (0);
}

// This function checks if the line has unclosed quotes
int	ft_check_unvalid(char *line)
{
	int	i;
	int	dquotes;
	int	squotes;

	i = -1;
	dquotes = 0;
	squotes = 0;
	if (ft_check_unexpected(line) == 1)
		ft_putendl_fd("minishell: syntax error (unexpected token)", 2);
	if (ft_check_unexpected(line) == 1)
		return (1);
	while (line[++i])
	{
		if (line[i] == '\"' && squotes % 2 == 0)
			dquotes++;
		if (line[i] == '\'' && dquotes % 2 == 0)
			squotes++;
	}
	if (dquotes % 2 != 0 || squotes % 2 != 0)
	{
		ft_crtl_c(2);
		ft_putendl_fd("minishell: syntax error (unclosed quotes)", 2);
		return (1);
	}
	return (0);
}
