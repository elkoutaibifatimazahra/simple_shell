#include "shell.h"

/**
 * shh - main shell loop
 * @info: the parameter & return info struct
 * @av: the argument vector from main()
 *
 * Return: 0 on success, 1 on error, or error code
 */
int shh(pass_t *info, char **av)
{
	ssize_t r = 0;
	int builtin_ret = 0;

	while (r != -1 && builtin_ret != -2)
	{
		reset_info(info);
		if (intrctv_mode(info))
			pts("$ ");
		_eptchr(CLEAR_BUFFER);
		r = gt_input(info);
		if (r != -1)
		{
			confg_info(info, av);
			builtin_ret = check_bltn(info);
			if (builtin_ret == -1)
				fnd_cmd(info);
		}
		else if (intrctv_mode(info))
			pt_char('\n');
		freeinfo(info, 0);
	}
	wr_history(info);
	freeinfo(info, 1);
	if (!intrctv_mode(info) && info->prev_cmd_status)
		exit(info->prev_cmd_status);
	if (builtin_ret == -2)
	{
		if (info->exit_code == -1)
			exit(info->prev_cmd_status);
		exit(info->exit_code);
	}
	return (builtin_ret);
}

/**
 * check_bltn - finds a builtin command
 * @info: the parameter & return info struct
 *
 * Return: -1 if builtin not found,
 *			0 if builtin executed successfully,
 *			1 if builtin found but not successful,
 *			-2 if builtin signals exit()
 */
int check_bltn(pass_t *info)
{
	int i, built_in_ret = -1;
	bltn_t builtintbl[] = {
		{"exit", cmd_exit},
		{"env", cmd_env},
		{"help", cmd_help},
		{"history", cmd_histr},
		{"setenv", cmd_setenv},
		{"unsetenv", cmd_unsetenv},
		{"cd", cmd_cd},
		{"alias", cmd_alias},
		{NULL, NULL}
	};

	for (i = 0; builtintbl[i].tp; i++)
		if (strcompare(info->args[0], builtintbl[i].tp) == 0)
		{
			info->line_num++;
			built_in_ret = builtintbl[i].fnc(info);
			break;
		}
	return (built_in_ret);
}

/**
 * fnd_cmd - finds a command in PATH
 * @info: the parameter & return info struct
 *
 * Return: void
 */
void fnd_cmd(pass_t *info)
{
	char *path = NULL;
	int i, k;

	info->ex_path = info->args[0];
	if (info->countline_flag == 1)
	{
		info->line_num++;
		info->countline_flag = 0;
	}
	for (i = 0, k = 0; info->input[i]; i++)
		if (!is_dlm(info->input[i], " \t\n"))
			k++;
	if (!k)
		return;

	path = fnd_pth(info, gt_env(info, "PATH="), info->args[0]);
	if (path)
	{
		info->ex_path = path;
		frk_cmd(info);
	}
	else
	{
		if ((intrctv_mode(info) || gt_env(info, "PATH=")
			|| info->args[0][0] == '/') && is_a_cmd(info, info->args[0]))
			frk_cmd(info);
		else if (*(info->input) != '\n')
		{
			info->prev_cmd_status = 127;
			prnt_error(info, "not found\n");
		}
	}
}

/**
 * frk_cmd - forks a an exec thread to run cmd
 * @info: the parameter & return info struct
 *
 * Return: void
 */
void frk_cmd(pass_t *info)
{
	pid_t child_pid;

	child_pid = fork();
	if (child_pid == -1)
	{
		/* TODO: PUT ERROR FUNCTION */
		perror("Error:");
		return;
	}
	if (child_pid == 0)
	{
		if (execve(info->ex_path, info->args, gt_environ(info)) == -1)
		{
			freeinfo(info, 1);
			if (errno == EACCES)
				exit(126);
			exit(1);
		}
		/* TODO: PUT ERROR FUNCTION */
	}
	else
	{
		wait(&(info->prev_cmd_status));
		if (WIFEXITED(info->prev_cmd_status))
		{
			info->prev_cmd_status = WEXITSTATUS(info->prev_cmd_status);
			if (info->prev_cmd_status == 126)
				prnt_error(info, "Permission denied\n");
		}
	}
}