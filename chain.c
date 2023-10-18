#include "shell.h"

/**
 * is_cmd_chain - test if current char in buffer is a chain delimeter
 * @info: the parameter struct
 * @buf: the char buffer
 * @p: address of current position in buf
 *
 * Return: 1 if chain delimeter, 0 otherwise
 */
int is_cmd_chain(pass_t *info, char *buf, size_t *p)
{
	size_t j = *p;

	if (buf[j] == '|' && buf[j + 1] == '|')
	{
		buf[j] = 0;
		j++;
		info->chain_type = CMD_CONDITION_OR;
	}
	else if (buf[j] == '&' && buf[j + 1] == '&')
	{
		buf[j] = 0;
		j++;
		info->chain_type = CMD_CONDITION_AND;
	}
	else if (buf[j] == ';') /* found end of this command */
	{
		buf[j] = 0; /* replace semicolon with null */
		info->chain_type = CMD_SEQUENTIAL;
	}
	else
		return (0);
	*p = j;
	return (1);
}

/**
 * chck_chain - checks we should continue chaining based on last status
 * @info: the parameter struct
 * @buf: the char buffer
 * @p: address of current position in buf
 * @i: starting position in buf
 * @len: length of buf
 *
 * Return: Void
 */
void chck_chain(pass_t *info, char *buf, size_t *p, size_t i, size_t len)
{
	size_t j = *p;

	if (info->chain_type == CMD_CONDITION_AND)
	{
		if (info->prev_cmd_status)
		{
			buf[i] = 0;
			j = len;
		}
	}
	if (info->chain_type == CMD_CONDITION_OR)
	{
		if (!info->prev_cmd_status)
		{
			buf[i] = 0;
			j = len;
		}
	}

	*p = j;
}

/**
 * rplc_alias - replaces an aliases in the tokenized string
 * @info: the parameter struct
 *
 * Return: 1 if replaced, 0 otherwise
 */
int rplc_alias(pass_t *info)
{
	int i;
	lst_t *node;
	char *p;

	for (i = 0; i < 10; i++)
	{
		node = srch_node_start(info->alias_list, info->args[0], '=');
		if (!node)
			return (0);
		free(info->args[0]);
		p = str_char(node->text, '=');
		if (!p)
			return (0);
		p = dup_str(p + 1);
		if (!p)
			return (0);
		info->args[0] = p;
	}
	return (1);
}

/**
 * rplc_vars - replaces vars in the tokenized string
 * @info: the parameter struct
 *
 * Return: 1 if replaced, 0 otherwise
 */
int rplc_vars(pass_t *info)
{
	int i = 0;
	lst_t *node;

	for (i = 0; info->args[i]; i++)
	{
		if (info->args[i][0] != '$' || !info->args[i][1])
			continue;

		if (!strcompare(info->args[i], "$?"))
		{
			rplc_str(&(info->args[i]),
				dup_str(num_cnvrt(info->prev_cmd_status, 10, 0)));
			continue;
		}
		if (!strcompare(info->args[i], "$$"))
		{
			rplc_str(&(info->args[i]),
				dup_str(num_cnvrt(getpid(), 10, 0)));
			continue;
		}
		node = srch_node_start(info->local_env, &info->args[i][1], '=');
		if (node)
		{
			rplc_str(&(info->args[i]),
				dup_str(str_char(node->text, '=') + 1));
			continue;
		}
		rplc_str(&info->args[i], dup_str(""));

	}
	return (0);
}

/**
 * rplc_str - replaces string
 * @old: address of old string
 * @new: new string
 *
 * Return: 1 if replaced, 0 otherwise
 */
int rplc_str(char **old, char *new)
{
	free(*old);
	*old = new;
	return (1);
}