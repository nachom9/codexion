#include "codexion.h"

t_coder	*create_coder(int id)
{
	t_coder	*node;

	node = malloc(sizeof(t_coder));
	if (!node)
		return (NULL);
	node->id = id;
	return (node);
}

t_dongle	*create_dongle(int id)
{
	t_dongle	*node;

	node = malloc(sizeof(t_dongle));
	if (!node)
		return (NULL);
	node->id = id;
	return (node);
}