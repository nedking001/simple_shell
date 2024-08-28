#include "shell.h"

/**
 * input_buff - buffers chained commands
 * @info: parameter struct
 * @buf: address of buffer
 * @len: address of len var
 *
 * Return: bytes read
 */
ssize_t input_buff(info_t *info, char **buf, size_t *len)
{
	ssize_t p = 0;
	size_t len_p = 0;

	if (!*len) /* if nothing left in the buffer, fill it */
	{
		/*bfree((void **)info->cmd_buf);*/
		free(*buf);
		*buf = NULL;
		signal(SIGINT, sigintHandler);
#if USE_GETLINE
		p = getline(buf, &len_p, stdin);
#else
		p = _getline(info, buf, &len_p);
#endif
		if (p > 0)
		{
			if ((*buf)[p - 1] == '\n')
			{
				(*buf)[p - 1] = '\0'; /* remove trailing newline */
				p--;
			}
			info->linecount_flag = 1;
			delete_comments(*buf);
			build_history(info, *buf, info->histcount++);
			/* if (_strchr(*buf, ';')) is this a command chain? */
			{
				*len = p;
				info->cmd_buf = buf;
			}
		}
	}
	return (p);
}

/**
 * get_input - gets a line minus the newline
 * @info: parameter struct
 *
 * Return: bytes read
 */
ssize_t get_input(info_t *info)
{
	static char *buf; /* the ';' command chain buffer */
	static size_t i, j, len;
	ssize_t a = 0;
	char **buf_p = &(info->arg), *p;

	_putchar(BUF_FLUSH);
	a = input_buff(info, &buf, &len);
	if (a == -1) /* EOF */
		return (-1);
	if (len)	/* we have commands left in the chain buffer */
	{
		j = i; /* init new iterator to current buf position */
		p = buf + i; /* get pointer for return */

		check_chain(info, buf, &j, i, len);
		while (j < len) /* iterate to semicolon or end */
		{
			if (is_chain(info, buf, &j))
				break;
			j++;
		}

		i = j + 1; /* increment past nulled ';'' */
		if (i >= len) /* reached end of buffer? */
		{
			i = len = 0; /* reset position and length */
			info->cmd_buf_type = CMD_NORM;
		}

		*buf_p = p; /* pass back pointer to current command position */
		return (_strlen(p)); /* return length of current command */
	}

	*buf_p = buf; /* else not a chain, pass back buffer from _getline() */
	return (a); /* return length of buffer from _getline() */
}

/**
 * read_buff - reads a buffer
 * @info: parameter struct
 * @buf: buffer
 * @i: size
 *
 * Return: r
 */
ssize_t read_buff(info_t *info, char *buf, size_t *i)
{
	ssize_t p = 0;

	if (*i)
		return (0);
	p = read(info->readfd, buf, READ_BUF_SIZE);
	if (p >= 0)
		*i = p;
	return (p);
}

/**
 * _getline - gets the next line of input
 * @info: parameter struct
 * @ptr: address of pointer to buffer
 * @length: size of preallocated ptr buffer if not NULL
 *
 * Return: s
 */
int _getline(info_t *info, char **ptr, size_t *length)
{
	static char buf[READ_BUF_SIZE];
	static size_t i, len;
	size_t a;
	ssize_t r = 0, s = 0;
	char *p = NULL, *new_p = NULL, *c;

	p = *ptr;
	if (p && length)
		s = *length;
	if (i == len)
		i = len = 0;

	r = read_buff(info, buf, &len);
	if (r == -1 || (r == 0 && len == 0))
		return (-1);

	c = _strchr(buf + i, '\n');
	a = c ? 1 + (unsigned int)(c - buf) : len;
	new_p = _realloc(p, s, s ? s + a : a + 1);
	if (!new_p) /* MALLOC FAILURE! */
		return (p ? free(p), -1 : -1);

	if (s)
		_strncat(new_p, buf + i, a - i);
	else
		_strncpy(new_p, buf + i, a - i + 1);

	s += a - i;
	i = a;
	p = new_p;

	if (length)
		*length = s;
	*ptr = p;
	return (s);
}

/**
 * sigintHandler - blocks ctrl-C
 * @sig_num: the signal number
 *
 * Return: void
 */
void sigintHandler(__attribute__((unused))int sig_num)
{
	_puts("\n");
	_puts("$ ");
	_putchar(BUF_FLUSH);
}
