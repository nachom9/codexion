#include "codexion.h"

void	free_all(t_params *params)
{
	int	i;

	i = 0;
	while (i < params->number_of_coders)
	{
		pthread_join(params->coders->coders[i]->thread, NULL);
		free(params->coders->coders[i]);
		free(params->dongles->dongles[i]);
		i++;
	}
	free(params->coders->coders);
	free(params->dongles->dongles);
	free(params->coders);
	free(params->dongles);
	free(params);
}

void	set_threads(t_params *params, t_coders *coders, t_dongles *dongles)
{
	int	i;
	int	scheduler;

	scheduler = check_scheduler(params->scheduler);
	i = 0;
	if (params->number_of_coders == 1)
	{
		free_all(params);
		return ;
	}
	pthread_create(&params->death_thread, NULL, set_death_algorithm, params);
	while (i < params->number_of_coders)
	{
		if (scheduler == 1)
			pthread_create(&coders->coders[i]->thread,
				NULL, ft_fifo, coders->coders[i]);
		if (scheduler == 2)
			pthread_create(&coders->coders[i]->thread,
				NULL, ft_edf, coders->coders[i]);
		i++;
	}
	pthread_join(params->death_thread, NULL);
	free_all(params);
}

int	main(int argc, char *argv[])
{
	t_params	*params;
	t_coders	*coders;
	t_dongles	*dongles;
	int			i;
	int			params_checker;

	i = 0;
	if (argc != 9)
	{
		write(2, "Invalid number of arguments\n", 29);
		return (0);
	}
	params_checker = check_params(argv);
	if (params_checker > 0)
		return (print_error(params_checker));
	params = parse_params(argv);
	dongles = set_dongles(params->number_of_coders);
	params->dongles = dongles;
	coders = set_coders(params);
	params->coders = coders;
	set_threads(params, coders, dongles);
	return (0);
}
