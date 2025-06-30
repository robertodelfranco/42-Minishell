/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin_free.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 16:40:59 by rdel-fra          #+#    #+#             */
/*   Updated: 2025/06/30 14:35:01 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libft.h"

char	*ft_strjoin_free(char const *s1, char *s2)
{
	char	*newstr;
	int		len;
	int		i;
	int		j;

	len = (ft_strlen(s1) + ft_strlen(s2));
	newstr = (char *)malloc((len + 1) * sizeof(char));
	if (!newstr)
		return (NULL);
	i = -1;
	while (++i, s1[i] != '\0')
		newstr[i] = s1[i];
	j = 0;
	while (s2[j] != '\0')
		newstr[i++] = s2[j++];
	newstr[i] = '\0';
	free(s2);
	return (newstr);
}
