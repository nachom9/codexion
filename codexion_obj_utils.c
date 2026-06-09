#include "codexion.h"

t_coder	*create_coder(int id, t_params *params)
{
	t_coder	*node;

	node = malloc(sizeof(t_coder));
	if (!node)
		return (NULL);
	node->id = id;
	node->compiles = params->compiles_required;
	node->params = params;
	node->last_compile = 0;
	return (node);
}

t_dongle	*create_dongle(int id)
{
	t_dongle	*node;

	node = malloc(sizeof(t_dongle));
	if (!node)
		return (NULL);
	node->id = id;
    pthread_mutex_init(&node->mutex, NULL);
	return (node);
}