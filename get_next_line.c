#include "get_next_line.h"

static char *read_and_join(int fd, char *stash)
{
    char buffer[BUFFER_SIZE + 1];
    char *tmp;
    int byte = 1;

    while (!ft_strchr(stash, '\n') && (byte = read(fd, buffer, BUFFER_SIZE)) > 0)
    {
        buffer[byte] = '\0';
        tmp = stash;
        stash = ft_strjoin(tmp, buffer);
        free(tmp);
        if (!stash)
            return (NULL);
    }
    if (byte < 0)
    {
        free(stash);
        return (NULL);
    }
    return (stash);
}

static char *extract_line(char *stash)
{
    int i = 0;

    if (!stash || !stash[0])
        return (NULL);

    while (stash[i] && stash[i] != '\n')
        i++;
    if (stash[i] == '\n')
        i++;

    return ft_substr(stash, 0, i);
}

static char *clean_stash(char *stash)
{
    int i = 0;
    char *new_stash;

    while (stash[i] && stash[i] != '\n')
        i++;
    if (!stash[i])
    {
        free(stash);
        return (NULL);
    }
    new_stash = ft_strdup(stash + i + 1);
    free(stash);
    return new_stash;
}

char *get_next_line(int fd)
{
    static char *stash;
    char *line;

    if (fd < 0 || BUFFER_SIZE <= 0)
        return (NULL);

    if (!stash)
        stash = ft_strdup("");

    stash = read_and_join(fd, stash);
    if (!stash)
        return (NULL);

    line = extract_line(stash);
	if (!line)
	{
	    free(stash);
	    stash = NULL;
	    return (NULL);
	}
    stash = clean_stash(stash);

    return (line);
}
