#include "shell.h"

/**
 * cmd_histr - displays the history list, one command by line, preceded
 *              with line numbers, starting at 0.
 * @info: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 *  Return: Always 0
 */
int cmd_histr(pass_t *info)
{
	show_list(info->cmd_history);
	return (0);
}

/**
 * unset_alias - sets alias to string
 * @info: parameter struct
 * @str: the string alias
 *
 * Return: Always 0 on success, 1 on error
 */
int unset_alias(pass_t *info, char *str)
{
	char *p, c;
	int ret;

	p = str_char(str, '=');
	if (!p)
		return (1);
	c = *p;
	*p = 0;
	ret = rm_node_index(&(info->alias_list),
		gt_node_index(info->alias_list, srch_node_start(info->alias_list, str, -1)));
	*p = c;
	return (ret);
}

/**
 * set_alias - sets alias to string
 * @info: parameter struct
 * @str: the string alias
 *
 * Return: Always 0 on success, 1 on error
 */
int set_alias(pass_t *info, char *str)
{
	char *p;

	p = str_char(str, '=');
	if (!p)
		return (1);
	if (!*++p)
		return (unset_alias(info, str));

	unset_alias(info, str);
	return (append_node(&(info->alias_list), str, 0) == NULL);
}

/**
 * print_alias - prints an alias string
 * @node: the alias node
 *
 * Return: Always 0 on success, 1 on error
 */
int print_alias(lst_t *node)
{
	char *p = NULL, *a = NULL;

	if (node)
	{
		p = str_char(node->text, '=');
		for (a = node->text; a <= p; a++)
			pt_char(*a);
		pt_char('\'');
		pts(p + 1);
		pts("'\n");
		return (0);
	}
	return (1);
}

/**
 * cmd_alias - mimics the alias builtin (man alias)
 * @info: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 *  Return: Always 0
 */
int cmd_alias(pass_t *info)
{
	int i = 0;
	char *p = NULL;
	lst_t *node = NULL;

	if (info->arg_c == 1)
	{
		node = info->alias_list;
		while (node)
		{
			print_alias(node);
			node = node->nxt;
		}
		return (0);
	}
	for (i = 1; info->args[i]; i++)
	{
		p = str_char(info->args[i], '=');
		if (p)
			set_alias(info, info->args[i]);
		else
			print_alias(srch_node_start(info->alias_list, info->args[i], '='));
	}

	return (0);
}