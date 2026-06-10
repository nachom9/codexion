#include "codexion.h"

int	check_scheduler(char *scheduler)
{
	char	*edf;
	char	*fifo;

	edf = "edf";
	fifo = "fifo";
	if (!strcmp(scheduler, fifo))
		return (1);
	if (!strcmp(scheduler, edf))
		return (2);
	return (0);
}

long	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000L + tv.tv_usec / 1000);
}

t_params	*parse_params(char **args)
{
	t_params	*node;

	node = malloc(sizeof(t_params));
	if (!node)
		return (NULL);
	node->number_of_coders = atoi(args[1]);
	node->time_to_burnout = atol(args[2]);
	node->time_to_compile = atoi(args[3]);
	node->time_to_debug = atoi(args[4]);
	node->time_to_refactor = atoi(args[5]);
	node->compiles_required = atoi(args[6]);
	node->dongle_cooldown = atoi(args[7]);
	node->scheduler = args[8];
	node->start_time = get_time();
	node->state = 1;
	node->coders = NULL;
	node->dongles = NULL;
	pthread_mutex_init(&node->mutex, NULL);
	return (node);
}

t_dongles	*set_dongles(int nb_of_coders)
{
	int			i;
	t_dongles	*all_dongles;
	t_dongle	*dongle;

	i = 0;
	all_dongles = malloc(sizeof(t_dongles));
	if (!all_dongles)
		return (NULL);
	all_dongles->dongles = malloc(sizeof(t_dongle *) * nb_of_coders);
	if (!all_dongles->dongles)
		return (NULL);
	while (i < nb_of_coders)
	{
		dongle = create_dongle(i + 1);
		if (!dongle)
			return (NULL);
		all_dongles->dongles[i] = dongle;
		i++;
	}
	return (all_dongles);
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
