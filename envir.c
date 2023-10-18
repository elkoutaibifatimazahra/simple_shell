#include "shell.h"

/**
 * cmd_env - prints the current environment
 * @info: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 * Return: Always 0
 */
int cmd_env(pass_t *info)
{
	prnt_list_str(info->local_env);
	return (0);
}

/**
 * gt_env - gets the value of an environ variable
 * @info: Structure containing potential arguments. Used to maintain
 * @name: env var name
 *
 * Return: the value
 */
char *gt_env(pass_t *info, const char *name)
{
	lst_t *node = info->local_env;
	char *p;

	while (node)
	{
		p = strts_wth(node->text, name);
		if (p && *p)
			return (p);
		node = node->nxt;
	}
	return (NULL);
}

/**
 * cmd_setenv - Initialize a new environment variable,
 *             or modify an existing one
 * @info: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 *  Return: Always 0
 */
int cmd_setenv(pass_t *info)
{
	if (info->arg_c != 3)
	{
		_epts("Incorrect number of arguements\n");
		return (1);
	}
	if (add_env(info, info->args[1], info->args[2]))
		return (0);
	return (1);
}

/**
 * cmd_unsetenv - Remove an environment variable
 * @info: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 *  Return: Always 0
 */
int cmd_unsetenv(pass_t *info)
{
	int i;

	if (info->arg_c == 1)
	{
		_epts("Too few arguements.\n");
		return (1);
	}
	for (i = 1; i <= info->arg_c; i++)
		rm_env(info, info->args[i]);

	return (0);
}

/**
 * intlz_env_list - populates env linked list
 * @info: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 * Return: Always 0
 */
int intlz_env_list(pass_t *info)
{
	lst_t *node = NULL;
	size_t i;

	for (i = 0; environ[i]; i++)
		append_node(&node, environ[i], 0);
	info->local_env = node;
	return (0);
}