#include "shell.h"

/**
 * display_history - displays the history list, one command per line, preceded
 *                   by line numbers starting at 0.
 * @info: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 * Return: Always 0
 */
int display_history(info_t *info)
{
	print_list(info->history);
	return (0);
}

/**
 * unset_alias_cmd - unsets an alias by removing it from the alias list.
 * @info: Parameter struct
 * @str: The alias string
 *
 * Return: Always 0 on success, 1 on error
 */
int unset_alias_cmd(info_t *info, char *str)
{
	char *p, c;
	int ret;

	p = _strchr(str, '=');
	if (!p)
		return (1);
	c = *p;
	*p = 0;
	ret = delete_node_at_index(&(info->alias),
			get_node_index(info->alias, node_starts_with(info->alias, str, -1)));
	*p = c;
	return (ret);
}

/**
 * set_alias_cmd - sets an alias by adding it to the alias list.
 * @info: Parameter struct
 * @str: The alias string
 *
 * Return: Always 0 on success, 1 on error
 */
int set_alias_cmd(info_t *info, char *str)
{
	char *p;

	p = _strchr(str, '=');
	if (!p)
		return (1);
	if (!*++p)
		return (unset_alias_cmd(info, str));

	unset_alias_cmd(info, str);
	return (add_node_end(&(info->alias), str, 0) == NULL);
}

/**
 * print_alias_cmd - prints an alias string.
 * @node: The alias node
 *
 * Return: Always 0 on success, 1 on error
 */
int print_alias_cmd(list_t *node)
{
	char *p = NULL, *a = NULL;

	if (node)
	{
		p = _strchr(node->str, '=');
		for (a = node->str; a <= p; a++)
		_putchar(*a);
		_putchar('\'');
		_puts(p + 1);
		_puts("'\n");
		return (0);
	}

	return (1);
}

/**
 * alias_cmd - mimics the alias builtin command (man alias).
 * @info: Structure containing potential arguments. Used to maintain
 *         constant function prototype.
 * Return: Always 0
 */
int alias_cmd(info_t *info)
{
	int i = 0;
	char *p = NULL;
	list_t *node = NULL;

	if (info->argc == 1)
	{
		node = info->alias;
		while (node)
		{
			print_alias_cmd(node);
			node = node->next;
		}
		return (0);
	}
	for (i = 1; info->argv[i]; i++)
	{
		p = _strchr(info->argv[i], '=');
		if (p)
			set_alias_cmd(info, info->argv[i]);
		else
			print_alias_cmd(node_starts_with(info->alias, info->argv[i], '='));
	}

	return (0);
}
