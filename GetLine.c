#include "shell.h"

/**
 * input_buf - buffers chained commands
 * @info: parameter struct
 * @buf: address of buffer
 * @len: address of len var
 *
 * Return: bytes read
 */
ssize_t input_buf(pass_t *info, char **buf, size_t *len)
{
	ssize_t r = 0;
	size_t len_p = 0;

	if (!*len) /* if nothing left in the buffer, fill it */
	{
		/*buffree((void **)info->command_chain);*/
		free(*buf);
		*buf = NULL;
		signal(SIGINT, handle_sigint);
#if SYS_GETLINE
		r = getline(buf, &len_p, stdin);
#else
		r = _gtline(info, buf, &len_p);
#endif
		if (r > 0)
		{
			if ((*buf)[r - 1] == '\n')
			{
				(*buf)[r - 1] = '\0'; /* remove trailing newline */
				r--;
			}
			info->countline_flag = 1;
			strip_comments(*buf);
			bld_history_list(info, *buf, info->history_line_count++);
			/* if (str_char(*buf, ';')) is this a command chain? */
			{
				*len = r;
				info->command_chain = buf;
			}
		}
	}
	return (r);
}

/**
 * gt_input - gets a line minus the newline
 * @info: parameter struct
 *
 * Return: bytes read
 */
ssize_t gt_input(pass_t *info)
{
	static char *buf; /* the ';' command chain buffer */
	static size_t i, j, len;
	ssize_t r = 0;
	char **buf_p = &(info->input), *p;

	pt_char(CLEAR_BUFFER);
	r = input_buf(info, &buf, &len);
	if (r == -1) /* EOF */
		return (-1);
	if (len)	/* we have commands left in the chain buffer */
	{
		j = i; /* init new iterator to current buf position */
		p = buf + i; /* get pointer for return */

		chck_chain(info, buf, &j, i, len);
		while (j < len) /* iterate to semicolon or end */
		{
			if (is_cmd_chain(info, buf, &j))
				break;
			j++;
		}

		i = j + 1; /* increment past nulled ';'' */
		if (i >= len) /* reached end of buffer? */
		{
			i = len = 0; /* reset position and length */
			info->chain_type = CMD_STANDARD;
		}

		*buf_p = p; /* pass back pointer to current command position */
		return (strlength(p)); /* return length of current command */
	}

	*buf_p = buf; /* else not a chain, pass back buffer from _gtline() */
	return (r); /* return length of buffer from _gtline() */
}

/**
 * read_buf - reads a buffer
 * @info: parameter struct
 * @buf: buffer
 * @i: size
 *
 * Return: r
 */
ssize_t read_buf(pass_t *info, char *buf, size_t *i)
{
	ssize_t r = 0;

	if (*i)
		return (0);
	r = read(info->input_fd, buf, INPUT_BUF_SIZE);
	if (r >= 0)
		*i = r;
	return (r);
}

/**
 * _gtline - gets the next line of input from STDIN
 * @info: parameter struct
 * @ptr: address of pointer to buffer, preallocated or NULL
 * @length: size of preallocated ptr buffer if not NULL
 *
 * Return: s
 */
int _gtline(pass_t *info, char **ptr, size_t *length)
{
	static char buf[INPUT_BUF_SIZE];
	static size_t i, len;
	size_t k;
	ssize_t r = 0, s = 0;
	char *p = NULL, *new_p = NULL, *c;

	p = *ptr;
	if (p && length)
		s = *length;
	if (i == len)
		i = len = 0;

	r = read_buf(info, buf, &len);
	if (r == -1 || (r == 0 && len == 0))
		return (-1);

	c = str_char(buf + i, '\n');
	k = c ? 1 + (unsigned int)(c - buf) : len;
	new_p = realloc_mem(p, s, s ? s + k : k + 1);
	if (!new_p) /* MALLOC FAILURE! */
		return (p ? free(p), -1 : -1);

	if (s)
		str_combine_n(new_p, buf + i, k - i);
	else
		str_copy_n(new_p, buf + i, k - i + 1);

	s += k - i;
	i = k;
	p = new_p;

	if (length)
		*length = s;
	*ptr = p;
	return (s);
}

/**
 * handle_sigint - blocks ctrl-C
 * @sig_num: the signal number
 *
 * Return: void
 */
void handle_sigint(__attribute__((unused))int sig_num)
{
	pts("\n");
	pts("$ ");
	pt_char(CLEAR_BUFFER);
}