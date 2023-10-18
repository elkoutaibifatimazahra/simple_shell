#include "shell.h"

/**
 * access_history_file - gets the history file
 * @info: parameter struct
 *
 * Return: allocated string containg history file
 */

char *access_history_file(pass_t *info)
{
	char *buf, *dir;

	dir = gt_env(info, "HOME=");
	if (!dir)
		return (NULL);
	buf = malloc(sizeof(char) * (strlength(dir) + strlength(HISTORY_FILE) + 2));
	if (!buf)
		return (NULL);
	buf[0] = 0;
	str_copy(buf, dir);
	strcombine(buf, "/");
	strcombine(buf, HISTORY_FILE);
	return (buf);
}

/**
 * wr_history - creates a file, or appends to an existing file
 * @info: the parameter struct
 *
 * Return: 1 on success, else -1
 */
int wr_history(pass_t *info)
{
	ssize_t fd;
	char *filename = access_history_file(info);
	lst_t *node = NULL;

	if (!filename)
		return (-1);

	fd = open(filename, O_CREAT | O_TRUNC | O_RDWR, 0644);
	free(filename);
	if (fd == -1)
		return (-1);
	for (node = info->cmd_history; node; node = node->nxt)
	{
		_ptsfd(node->text, fd);
		_ptfd('\n', fd);
	}
	_ptfd(CLEAR_BUFFER, fd);
	close(fd);
	return (1);
}

/**
 * rd_history - reads history from file
 * @info: the parameter struct
 *
 * Return: history_line_count on success, 0 otherwise
 */
int rd_history(pass_t *info)
{
	int i, last = 0, linecount = 0;
	ssize_t fd, rdlen, fsize = 0;
	struct stat st;
	char *buf = NULL, *filename = access_history_file(info);

	if (!filename)
		return (0);

	fd = open(filename, O_RDONLY);
	free(filename);
	if (fd == -1)
		return (0);
	if (!fstat(fd, &st))
		fsize = st.st_size;
	if (fsize < 2)
		return (0);
	buf = malloc(sizeof(char) * (fsize + 1));
	if (!buf)
		return (0);
	rdlen = read(fd, buf, fsize);
	buf[fsize] = 0;
	if (rdlen <= 0)
		return (free(buf), 0);
	close(fd);
	for (i = 0; i < fsize; i++)
		if (buf[i] == '\n')
		{
			buf[i] = 0;
			bld_history_list(info, buf + last, linecount++);
			last = i + 1;
		}
	if (last != i)
		bld_history_list(info, buf + last, linecount++);
	free(buf);
	info->history_line_count = linecount;
	while (info->history_line_count-- >= MAX_HISTORY)
		rm_node_index(&(info->cmd_history), 0);
	history_renumber(info);
	return (info->history_line_count);
}

/**
 * bld_history_list - adds entry to a history linked list
 * @info: Structure containing potential arguments. Used to maintain
 * @buf: buffer
 * @linecount: the history linecount, history_line_count
 *
 * Return: Always 0
 */
int bld_history_list(pass_t *info, char *buf, int linecount)
{
	lst_t *node = NULL;

	if (info->cmd_history)
		node = info->cmd_history;
	append_node(&node, buf, linecount);

	if (!info->cmd_history)
		info->cmd_history = node;
	return (0);
}

/**
 * history_renumber - renumbers the history linked list after changes
 * @info: Structure containing potential arguments. Used to maintain
 *
 * Return: the new history_line_count
 */
int history_renumber(pass_t *info)
{
	lst_t *node = info->cmd_history;
	int i = 0;

	while (node)
	{
		node->value = i++;
		node = node->nxt;
	}
	return (info->history_line_count = i);
}