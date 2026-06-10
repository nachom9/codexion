#include "codexion.h"

int	check_state(t_params *params)
{
	int	state;

	pthread_mutex_lock(&params->mutex);
	state = params->state;
	pthread_mutex_unlock(&params->mutex);
	return (state);
}

void	end_state(t_params *params)
{
	pthread_mutex_lock(&params->mutex);
	params->state = 0;
	pthread_mutex_unlock(&params->mutex);
}
