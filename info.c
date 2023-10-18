#include "shell.h"

/**
 * reset_info - initializes pass_t struct
 * @info: struct address
 */
void reset_info(pass_t *info)
{
	info->input = NULL;
	info->args = NULL;
	info->ex_path = NULL;
	info->arg_c = 0;
}

/**
 * confg_info - initializes pass_t struct
 * @info: struct address
 * @av: argument vector
 */
void confg_info(pass_t *info, char **av)
{
	int i = 0;

	info->prog_name = av[0];
	if (info->input)
	{
		info->args = str_to_words(info->input, " \t");
		if (!info->args)
		{

			info->args = malloc(sizeof(char *) * 2);
			if (info->args)
			{
				info->args[0] = dup_str(info->input);
				info->args[1] = NULL;
			}
		}
		for (i = 0; info->args && info->args[i]; i++)
			;
		info->arg_c = i;

		rplc_alias(info);
		rplc_vars(info);
	}
}

/**
 * freeinfo - frees pass_t struct fields
 * @info: struct address
 * @all: true if freeing all fields
 */
void freeinfo(pass_t *info, int all)
{
	free_mem(info->args);
	info->args = NULL;
	info->ex_path = NULL;
	if (all)
	{
		if (!info->command_chain)
			free(info->input);
		if (info->local_env)
			frlist(&(info->local_env));
		if (info->cmd_history)
			frlist(&(info->cmd_history));
		if (info->alias_list)
			frlist(&(info->alias_list));
		free_mem(info->custom_environ);
			info->custom_environ = NULL;
		buffree((void **)info->command_chain);
		if (info->input_fd > 2)
			close(info->input_fd);
		pt_char(CLEAR_BUFFER);
	}
}