/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gecarval <gecarval@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 13:11:59 by gecarval          #+#    #+#             */
/*   Updated: 2024/12/15 06:08:13 by gecarval         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*result;
	int		i;
	int		j;
	size_t	total_len;

	j = 0;
	i = -1;
	if (s1 == NULL && s2 == NULL)
		return (NULL);
	total_len = ft_strlen(s1) + ft_strlen(s2);
	result = (char *)ft_calloc(total_len + 5, sizeof(char));
	if (result == NULL)
		return (NULL);
	while (s1 != NULL && s1[++i] != '\0')
		result[i] = ((unsigned char *)s1)[i];
	while (s2 != NULL && s2[j] != '\0')
		result[i++] = ((unsigned char *)s2)[j++];
	return (result);
}
