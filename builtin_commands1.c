#include "shell.h"

/**
 * my_exit - exits the shell
 * @info: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 *  Return: exits with a given exit status
 *         (0) if info.argv[0] != "exit"
 */
int my_exit(info_t *info)
{
	int exitcheck;

	if (info->argv[1])  /* If there is an exit arguement */
	{
		exitcheck = err_atoi(info->argv[1]);
		if (exitcheck == -1)
		{
			info->status = 2;
			print_err(info, "Illegal number: ");
			err_puts(info->argv[1]);
			err_putchar('\n');
			return (1);
		}
		info->err_num = err_atoi(info->argv[1]);
		return (-2);
	}
	info->err_num = -1;
	return (-2);
}

/**
 * my_cd - changes the current directory of the process
 * @info: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 *  Return: Always 0
 */
int my_cd(info_t *info)
{
	char *p, *dir, buffer[1024];
	int chdir_ret;

	p = getcwd(buffer, 1024);
	if (!p)
		_puts("TODO: >>getcwd failure emsg here<<\n");
	if (!info->argv[1])
	{
		dir = get_env(info, "HOME=");
		if (!dir)
			chdir_ret =
				chdir((dir = get_env(info, "PWD=")) ? dir : "/");
		else
			chdir_ret = chdir(dir);
	}
	else if (_strcmp(info->argv[1], "-") == 0)
	{
		if (!get_env(info, "OLDPWD="))
		{
			_puts(p);
			_putchar('\n');
			return (1);
		}
		_puts(get_env(info, "OLDPWD=")), _putchar('\n');
		chdir_ret =
			chdir((dir = get_env(info, "OLDPWD=")) ? dir : "/");
	}
	else
		chdir_ret = chdir(info->argv[1]);
	if (chdir_ret == -1)
	{
		print_err(info, "can't cd to ");
		err_puts(info->argv[1]), err_putchar('\n');
	}
	else
	{
		_setenv(info, "OLDPWD", get_env(info, "PWD="));
		_setenv(info, "PWD", getcwd(buffer, 1024));
	}
	return (0);
}

/**
 * my_help - changes the current directory of the process
 * @info: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 *  Return: Always 0
 */
int my_help(info_t *info)
{
	char **arg_arr;

	arg_arr = info->argv;
	_puts("help call works. Function not yet implemented \n");
	if (0)
		_puts(*arg_arr); /* temp att_unused workaround */
	return (0);
}
