#include "shell.h"

/**
 * my_history - displays the history list, one command by line, preceded
 *              with line numbers, starting at 0.
 * @info: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 *  Return: Always 0
 */
int my_history(info_t *info)
{
	print_list(info->history);
	return (0);
}

/**
 * reset_alias - sets alias to string
 * @info: parameter struct
 * @str: the string alias
 *
 * Return: Always 0 on success, 1 on error
 */
int reset_alias(info_t *info, char *str)
{
	char *p, d;
	int reset;

	p = _strchr(str, '=');
	if (!p)
		return (1);
	d = *p;
	*p = 0;
	reset = delete_node_at_index(&(info->alias),
		get_node_index(info->alias, node_starter(info->alias, str, -1)));
	*p = d;
	return (reset);
}

/**
 * set_alias - sets alias to string
 * @info: parameter struct
 * @str: the string alias
 *
 * Return: Always 0 on success, 1 on error
 */
int set_alias(info_t *info, char *str)
{
	char *p;

	p = _strchr(str, '=');
	if (!p)
		return (1);
	if (!*++p)
		return (reset_alias(info, str));

	reset_alias(info, str);
	return (add_node_end(&(info->alias), str, 0) == NULL);
}

/**
 * display_alias - prints an alias string
 * @node: the alias node
 *
 * Return: Always 0 on success, 1 on error
 */
int display_alias(list_t *node)
{
	char *p = NULL, *b = NULL;

	if (node)
	{
		p = _strchr(node->str, '=');
		for (b = node->str; b <= p; b++)
			_putchar(*b);
		_putchar('\'');
		_puts(p + 1);
		_puts("'\n");
		return (0);
	}
	return (1);
}

/**
 * my_alias - mimics the alias builtin (man alias)
 * @info: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 *  Return: Always 0
 */
int my_alias(info_t *info)
{
	int i = 0;
	char *p = NULL;
	list_t *node = NULL;

	if (info->argc == 1)
	{
		node = info->alias;
		while (node)
		{
			display_alias(node);
			node = node->next;
		}
		return (0);
	}
	for (i = 1; info->argv[i]; i++)
	{
		p = _strchr(info->argv[i], '=');
		if (p)
			set_alias(info, info->argv[i]);
		else
			display_alias(node_starter(info->alias, info->argv[i], '='));
	}

	return (0);
}
