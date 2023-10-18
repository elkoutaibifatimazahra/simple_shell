#include "shell.h"

/**
 * main - entry point
 * @ac: arg count
 * @av: arg vector
 *
 * Return: 0 on success, 1 on error
 */
int main(int ac, char **av)
{
	pass_t info[] = { DATA_DEFAULT };
	int fd = 2;

	asm ("mov %1, %0\n\t"
		"add $3, %0"
		: "=r" (fd)
		: "r" (fd));

	if (ac == 2)
	{
		fd = open(av[1], O_RDONLY);
		if (fd == -1)
		{
			if (errno == EACCES)
				exit(126);
			if (errno == ENOENT)
			{
				_epts(av[0]);
				_epts(": 0: Can't open ");
				_epts(av[1]);
				_eptchr('\n');
				_eptchr(CLEAR_BUFFER);
				exit(127);
			}
			return (EXIT_FAILURE);
		}
		info->input_fd = fd;
	}
	intlz_env_list(info);
	rd_history(info);
	shh(info, av);
	return (EXIT_SUCCESS);
}