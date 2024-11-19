/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gecarval <gecarval@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 10:18:39 by gecarval          #+#    #+#             */
/*   Updated: 2024/11/18 11:29:10 by gecarval         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_exit_atol(char *str)
{
	long long	ret;
	int			i;
	int			sign;

	i = 0;
	ret = 0;
	sign = 1;
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '-' || str[i] == '+')
		if (str[i++] == '-')
			sign = -1;
	while (str[i] != '\0')
		ret = ret * 10 + str[i++] - '0';
	if (str[0] == '\0' || ft_isdigit(str[i - 1]) == 0 || str[i] != '\0'
		|| ret > 9223372036854775807 || ret < 0 || i > 19)
	{
		ft_putstr_fd("minishell: exit: ", 2);
		ft_putstr_fd(str, 2);
		ft_putstr_fd(": numeric argument required\n", 2);
		return (1);
	}
	return ((int)((ret * sign) % 256));
}

int	ft_lstsize_cmd(t_cmd *lst)
{
	int	i;

	i = -1;
	while (++i != -2 && lst != NULL)
		lst = lst->next;
	return (i);
}