#include "shell.h"

/**
 * cmd_exit - exits the shell
 * @info: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 *  Return: exits with a given exit prev_cmd_status
 *         (0) if info.args[0] != "exit"
 */

int cmd_exit(pass_t *info)
{
	int exitcheck;

	if (info->args[1])  /* If there is an exit arguement */
	{
		exitcheck = error_str_to_int(info->args[1]);
		if (exitcheck == -1)
		{
			info->prev_cmd_status = 2;
			prnt_error(info, "Illegal number: ");
			_epts(info->args[1]);
			_eptchr('\n');
			return (1);
		}
		info->exit_code = error_str_to_int(info->args[1]);
		return (-2);
	}
	info->exit_code = -1;
	return (-2);
}



/**
 * cmd_cd - changes the current directory of the process
 * @info: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 *  Return: Always 0
 */
int cmd_cd(pass_t *info)
{
	char *s, *dir, buffer[1024];
	int chdir_ret;

	s = getcwd(buffer, 1024);
	if (!s)
		pts("TODO: >>getcwd failure emsg here<<\n");
	if (!info->args[1])
	{
		dir = gt_env(info, "HOME=");
		if (!dir)
			chdir_ret = /* TODO: what should this be? */
				chdir((dir = gt_env(info, "PWD=")) ? dir : "/");
		else
			chdir_ret = chdir(dir);
	}
	else if (strcompare(info->args[1], "-") == 0)
	{
		if (!gt_env(info, "OLDPWD="))
		{
			pts(s);
			pt_char('\n');
			return (1);
		}
		pts(gt_env(info, "OLDPWD=")), pt_char('\n');
		chdir_ret = /* TODO: what should this be? */
			chdir((dir = gt_env(info, "OLDPWD=")) ? dir : "/");
	}
	else
		chdir_ret = chdir(info->args[1]);
	if (chdir_ret == -1)
	{
		prnt_error(info, "can't cd to ");
		_epts(info->args[1]), _eptchr('\n');
	}
	else
	{
		add_env(info, "OLDPWD", gt_env(info, "PWD="));
		add_env(info, "PWD", getcwd(buffer, 1024));
	}
	return (0);
}

/**
 * cmd_help - changes the current directory of the process
 * @info: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 *  Return: Always 0
 */
int cmd_help(pass_t *info)
{
	char **arg_array;

	arg_array = info->args;
	pts("help call works. Function not yet implemented \n");
	if (0)
		pts(*arg_array); /* temp att_unused workaround */
	return (0);
}