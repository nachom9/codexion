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
	pthread_mutex_init(&node->mutex, NULL);
	return (node);
}

t_coders	*set_coders(t_params *params)
{
	int			i;
	t_coders	*all_coders;
	t_coder		*coder;

	i = 0;
	all_coders = malloc(sizeof(t_coders));
	if (!all_coders)
		return (NULL);
	all_coders->coders = malloc(sizeof(t_coder *) * params->number_of_coders);
	if (!all_coders->coders)
		return (NULL);
	if (!all_coders)
		return (NULL);
	while (i < params->number_of_coders)
	{
		coder = create_coder(i + 1, params);
		if (!coder)
			return (NULL);
		all_coders->coders[i] = coder;
		i++;
	}
	return (all_coders);
}
