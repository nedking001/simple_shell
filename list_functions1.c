#include "shell.h"

/**
 * add_node - Adds a node to the start of the list.
 * @head: Address of pointer to head node.
 * @str: String field of the node.
 * @num: Node index used by history.
 *
 * Return: Pointer to the new node, or NULL on failure.
 */
list_t *add_node(list_t **head, const char *str, int num)
{
	list_t *new_hd;

	if (!head)
		return (NULL);

	new_hd = malloc(sizeof(list_t));
	if (!new_hd)
		return (NULL);

	_memset((void *)new_hd, 0, sizeof(list_t));
	new_hd->num = num;

	if (str)
	{
		new_hd->str = _strdup(str);
		if (!new_hd->str)
		{
			free(new_hd);
			return (NULL);
		}
	}
	new_hd->next = *head;
	*head = new_hd;
	return (new_hd);
}

/**
 * add_node_end - Adds a node to the end of the list.
 * @head: Address of pointer to head node.
 * @str: String field of the node.
 * @num: Node index used by history.
 *
 * Return: Pointer to the new node, or NULL on failure.
 */
list_t *add_node_end(list_t **head, const char *str, int num)
{
	list_t *new_node, *node;

	if (!head)
		return (NULL);

	node = *head;
	new_node = malloc(sizeof(list_t));
	if (!new_node)
		return (NULL);

	_memset((void *)new_node, 0, sizeof(list_t));
	new_node->num = num;

	if (str)
	{
		new_node->str = _strdup(str);
		if (!new_node->str)
		{
			free(new_node);
			return (NULL);
		}
	}
	new_node->next = NULL;

	if (node)
	{
		while (node->next)
			node = node->next;
		node->next = new_node;
	}
	else
		*head = new_node;

	return (new_node);
}

/**
 * print_str_list - Prints only the str element of a list_t linked list.
 * @h: Pointer to the first node.
 *
 * Return: Size of the list.
 */
size_t print_str_list(const list_t *h)
{
	size_t i = 0;

	while (h)
	{
		_puts(h->str ? h->str : "(nil)");
		_puts("\n");
		h = h->next;
		i++;
	}
	return (i);
}

/**
 * delete_node_at_index - Deletes the node at the given index.
 * @head: Address of pointer to the first node.
 * @index: Index of the node to delete.
 *
 * Return: 1 on success, 0 on failure.
 */
int delete_node_at_index(list_t **head, unsigned int index)
{
	list_t *node, *pre_node;
	unsigned int i = 0;

	if (!head || !*head)
		return (0);

	if (index == 0)
	{
		node = *head;
		*head = (*head)->next;
		free(node->str);
		free(node);
		return (1);
	}

	node = *head;
	while (node)
	{
		if (i == index)
		{
			pre_node->next = node->next;
			free(node->str);
			free(node);
			return (1);
		}
		i++;
		pre_node = node;
		node = node->next;
	}
	return (0);
}

/**
 * free_node_list - Frees all nodes of a list.
 * @head_ptr: Address of pointer to the head node.
 *
 * Return: void.
 */
void free_node_list(list_t **head_ptr)
{
	list_t *node, *next_node;

	if (!head_ptr || !*head_ptr)
		return;

	node = *head_ptr;
	while (node)
	{
		next_node = node->next;
		free(node->str);
		free(node);
		node = next_node;
	}
	*head_ptr = NULL;
}
