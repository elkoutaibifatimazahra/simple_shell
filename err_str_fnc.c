#include "shell.h"

/**
 *_epts - prints an input string
 * @str: the string to be printed
 *
 * Return: Nothing
 */
void _epts(char *str)
{
	int i = 0;

	if (!str)
		return;
	while (str[i] != '\0')
	{
		_eptchr(str[i]);
		i++;
	}
}

/**
 * _eptchr - writes the character c to stderr
 * @c: The character to print
 *
 * Return: On success 1.
 * On error, -1 is returned, and errno is set appropriately.
 */
int _eptchr(char c)
{
	static int i;
	static char buf[OUTPUT_BUF_SIZE];

	if (c == CLEAR_BUFFER || i >= OUTPUT_BUF_SIZE)
	{
		write(2, buf, i);
		i = 0;
	}
	if (c != CLEAR_BUFFER)
		buf[i++] = c;
	return (1);
}

/**
 * _ptfd - writes the character c to given fd
 * @c: The character to print
 * @fd: The filedescriptor to write to
 *
 * Return: On success 1.
 * On error, -1 is returned, and errno is set appropriately.
 */
int _ptfd(char c, int fd)
{
	static int i;
	static char buf[OUTPUT_BUF_SIZE];

	if (c == CLEAR_BUFFER || i >= OUTPUT_BUF_SIZE)
	{
		write(fd, buf, i);
		i = 0;
	}
	if (c != CLEAR_BUFFER)
		buf[i++] = c;
	return (1);
}

/**
 *_ptsfd - prints an input string
 * @str: the string to be printed
 * @fd: the filedescriptor to write to
 *
 * Return: the number of chars put
 */
int _ptsfd(char *str, int fd)
{
	int i = 0;

	if (!str)
		return (0);
	while (*str)
	{
		i += _ptfd(*str++, fd);
	}
	return (i);
}