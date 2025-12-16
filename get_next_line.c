/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kriad <kriad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 17:08:49 by kriad             #+#    #+#             */
/*   Updated: 2025/11/24 17:45:11 by kriad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static char	*gnl_read(int fd, char *stash)
{
	char	*buffer;
	char	*tmp;
	int		byte;

	byte = 1;
	buffer = malloc((size_t)BUFFER_SIZE + 1);
	if (!buffer)
		return (NULL);
	while (!ft_strchr(stash, '\n'))
	{
		byte = read(fd, buffer, BUFFER_SIZE);
		if (byte <= 0)
			break ;
		buffer[byte] = '\0';
		tmp = stash;
		stash = ft_strjoin(tmp, buffer);
		free(tmp);
		if (!stash)
			return (free(buffer), NULL);
	}
	free(buffer);
	if (byte < 0)
		return (free(stash), NULL);
	return (stash);
}

static char	*gnl_extract(char *stash)
{
	int	i;

	i = 0;
	if (!stash || !stash[0])
		return (NULL);
	while (stash[i] && stash[i] != '\n')
		i++;
	if (stash[i] == '\n')
		i++;
	return (ft_substr(stash, 0, i));
}

static char	*gnl_clean(char *stash)
{
	char	*new;
	int		i;

	i = 0;
	while (stash[i] && stash[i] != '\n')
		i++;
	if (!stash[i])
		return (free(stash), NULL);
	new = ft_strdup(stash + i + 1);
	free(stash);
	return (new);
}

char	*get_next_line(int fd)
{
	static char	*stash;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	if (!stash)
		stash = ft_strdup("");
	stash = gnl_read(fd, stash);
	if (!stash)
		return (NULL);
	line = gnl_extract(stash);
	if (!line)
	{
		free(stash);
		stash = NULL;
		return (NULL);
	}
	stash = gnl_clean(stash);
	return (line);
}
