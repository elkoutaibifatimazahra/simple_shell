#include "shell.h"

/**
 * prepend_node - adds a node to the start of the list
 * @head: address of pointer to head node
 * @str: str field of node
 * @num: node index used by history
 *
 * Return: size of list
 */
lst_t *prepend_node(lst_t **head, const char *str, int num)
{
	lst_t *new_head;

	if (!head)
		return (NULL);
	new_head = malloc(sizeof(lst_t));
	if (!new_head)
		return (NULL);
	set_mem((void *)new_head, 0, sizeof(lst_t));
	new_head->value = num;
	if (str)
	{
		new_head->text = dup_str(str);
		if (!new_head->text)
		{
			free(new_head);
			return (NULL);
		}
	}
	new_head->nxt = *head;
	*head = new_head;
	return (new_head);
}

/**
 * add_node_end - adds a node to the end of the list
 * @head: address of pointer to head node
 * @str: str field of node
 * @num: node index used by history
 *
 * Return: size of list
 */
lst_t *append_node(lst_t **head, const char *str, int num)
{
	lst_t *new_node, *node;

	if (!head)
		return (NULL);

	node = *head;
	new_node = malloc(sizeof(lst_t));
	if (!new_node)
		return (NULL);
	set_mem((void *)new_node, 0, sizeof(lst_t));
	new_node->value = num;
	if (str)
	{
		new_node->text = dup_str(str);
		if (!new_node->text)
		{
			free(new_node);
			return (NULL);
		}
	}
	if (node)
	{
		while (node->nxt)
			node = node->nxt;
		node->nxt = new_node;
	}
	else
		*head = new_node;
	return (new_node);
}

/**
 * prnt_list_str - prints only the str element of a lst_t linked list
 * @h: pointer to first node
 *
 * Return: size of list
 */
size_t prnt_list_str(const lst_t *h)
{
	size_t i = 0;

	while (h)
	{
		pts(h->text ? h->text : "(nil)");
		pts("\n");
		h = h->nxt;
		i++;
	}
	return (i);
}

/**
 * rm_node_index - deletes node at given index
 * @head: address of pointer to first node
 * @index: index of node to delete
 *
 * Return: 1 on success, 0 on failure
 */
int rm_node_index(lst_t **head, unsigned int index)
{
	lst_t *node, *prev_node;
	unsigned int i = 0;

	if (!head || !*head)
		return (0);

	if (!index)
	{
		node = *head;
		*head = (*head)->nxt;
		free(node->text);
		free(node);
		return (1);
	}
	node = *head;
	while (node)
	{
		if (i == index)
		{
			prev_node->nxt = node->nxt;
			free(node->text);
			free(node);
			return (1);
		}
		i++;
		prev_node = node;
		node = node->nxt;
	}
	return (0);
}

/**
 * frlist - frees all nodes of a list
 * @head_ptr: address of pointer to head node
 *
 * Return: void
 */
void frlist(lst_t **head_ptr)
{
	lst_t *node, *next_node, *head;

	if (!head_ptr || !*head_ptr)
		return;
	head = *head_ptr;
	node = head;
	while (node)
	{
		next_node = node->nxt;
		free(node->text);
		free(node);
		node = next_node;
	}
	*head_ptr = NULL;
}