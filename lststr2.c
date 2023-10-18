#include "shell.h"

/**
 * list_length - determines length of linked list
 * @h: pointer to first node
 *
 * Return: size of list
 */
size_t list_length(const lst_t *h)
{
	size_t i = 0;

	while (h)
	{
		h = h->nxt;
		i++;
	}
	return (i);
}

/**
 * lst_to_strings - returns an array of strings of the list->text
 * @head: pointer to first node
 *
 * Return: array of strings
 */
char **listtostr(lst_t *head)
{
	lst_t *node = head;
	size_t i = list_length(head), j;
	char **strs;
	char *str;

	if (!head || !i)
		return (NULL);
	strs = malloc(sizeof(char *) * (i + 1));
	if (!strs)
		return (NULL);
	for (i = 0; node; node = node->nxt, i++)
	{
		str = malloc(strlength(node->text) + 1);
		if (!str)
		{
			for (j = 0; j < i; j++)
				free(strs[j]);
			free(strs);
			return (NULL);
		}

		str = str_copy(str, node->text);
		strs[i] = str;
	}
	strs[i] = NULL;
	return (strs);
}


/**
 * show_list - prints all elements of a lst_t linked list
 * @h: pointer to first node
 *
 * Return: size of list
 */
size_t show_list(const lst_t *h)
{
	size_t i = 0;

	while (h)
	{
		pts(num_cnvrt(h->value, 10, 0));
		pt_char(':');
		pt_char(' ');
		pts(h->text ? h->text : "(nil)");
		pts("\n");
		h = h->nxt;
		i++;
	}
	return (i);
}

/**
 * srch_node_start - returns node whose string starts with prefix
 * @node: pointer to list head
 * @prefix: string to match
 * @c: the next character after prefix to match
 *
 * Return: match node or null
 */
lst_t *srch_node_start(lst_t *node, char *prefix, char c)
{
	char *p = NULL;

	while (node)
	{
		p = strts_wth(node->text, prefix);
		if (p && ((c == -1) || (*p == c)))
			return (node);
		node = node->nxt;
	}
	return (NULL);
}

/**
 * gt_node_index - gets the index of a node
 * @head: pointer to list head
 * @node: pointer to the node
 *
 * Return: index of node or -1
 */
ssize_t gt_node_index(lst_t *head, lst_t *node)
{
	size_t i = 0;

	while (head)
	{
		if (head == node)
			return (i);
		head = head->nxt;
		i++;
	}
	return (-1);
}