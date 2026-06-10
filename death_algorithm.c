#include "codexion.h"

void	*death_algorithm(t_params *params, t_coder *coder, int i, int j)
{
	while (j < params->number_of_coders)
	{
		i = 0;
		j = 0;
		while (i < params->number_of_coders)
		{
			coder = params->coders->coders[i++];
			pthread_mutex_lock(&coder->mutex);
			if (coder->compiles == 0)
				j++;
			if (((get_time() - params->start_time) - coder->last_compile
					> params->time_to_burnout && coder->compiles > 0))
			{
				pthread_mutex_unlock(&coder->mutex);
				end_state(params);
				printf("%ld %d burned out\n",
					get_time() - params->start_time, coder->id);
				return (NULL);
			}
			pthread_mutex_unlock(&coder->mutex);
		}
		usleep(5000);
	}
	end_state(params);
	return (NULL);
}

void	*set_death_algorithm(void *arg)
{
	int			i;
	int			j;
	t_params	*params;
	t_coder		*coder;

	j = 0;
	i = 0;
	params = (t_params *)arg;
	death_algorithm(params, coder, i, j);
	return (NULL);
}
