/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gecarval <gecarval@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 13:11:59 by gecarval          #+#    #+#             */
/*   Updated: 2024/12/14 10:16:26 by gecarval         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s1)
{
	char	*s2;
	size_t	len;

	if (s1 == NULL)
		return (NULL);
	len = ft_strlen(s1);
	s2 = (char *)ft_calloc(len + 3, sizeof(char));
	if ((s2) == NULL)
		return (NULL);
	len = -1;
	while (s1[++len] != '\0')
		s2[len] = s1[len];
	s2[len] = '\0';
	return (s2);
}
