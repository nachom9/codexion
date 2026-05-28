#include "codexion.h"

t_coder	*create_coder(int id)
{
	t_coder	*node;
	int	state;

	node = malloc(sizeof(t_coder));
	if (!node)
		return (NULL);
	node->id = id;
    pthread_mutex_init(&node->mutex, NULL);
	return (node);
}

t_dongle	*create_dongle(int id)
{
	t_dongle	*node;

	node = malloc(sizeof(t_dongle));
	if (!node)
		return (NULL);
	node->state = 0;
	node->id = id;
	return (node);
}