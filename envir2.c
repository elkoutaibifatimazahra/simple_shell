#include "shell.h"

/**
 * gt_environ - returns the string array copy of our environ
 * @info: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 * Return: Always 0
 */
char **gt_environ(pass_t *info)
{
	if (!info->custom_environ || info->env_modified)
	{
		info->custom_environ = listtostr(info->local_env);
		info->env_modified = 0;
	}

	return (info->custom_environ);
}

/**
 * rm_env - Remove an environment variable
 * @info: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 *  Return: 1 on delete, 0 otherwise
 * @var: the string env var property
 */
int rm_env(pass_t *info, char *var)
{
	lst_t *node = info->local_env;
	size_t i = 0;
	char *p;

	if (!node || !var)
		return (0);

	while (node)
	{
		p = strts_wth(node->text, var);
		if (p && *p == '=')
		{
			info->env_modified =  rm_node_index(&(info->local_env), i);
			i = 0;
			node = info->local_env;
			continue;
		}
		node = node->nxt;
		i++;
	}
	return (info->env_modified);
}

/**
 * add_env - Initialize a new environment variable,
 *             or modify an existing one
 * @info: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 * @var: the string env var property
 * @value: the string env var value
 *  Return: Always 0
 */
int add_env(pass_t *info, char *var, char *value)
{
	char *buf = NULL;
	lst_t *node;
	char *p;

	if (!var || !value)
		return (0);

	buf = malloc(strlength(var) + strlength(value) + 2);
	if (!buf)
		return (1);
	str_copy(buf, var);
	strcombine(buf, "=");
	strcombine(buf, value);
	node = info->local_env;
	while (node)
	{
		p = strts_wth(node->text, var);
		if (p && *p == '=')
		{
			free(node->text);
			node->text = buf;
			info->env_modified = 1;
			return (0);
		}
		node = node->nxt;
	}
	append_node(&(info->local_env), buf, 0);
	free(buf);
	info->env_modified = 1;
	return (0);
}