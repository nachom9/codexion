#include "codexion.h"

int	ft_min(t_coder *coder)
{
	int		i;
	long	last_compile;

	pthread_mutex_lock(&coder->mutex);
	last_compile = coder->last_compile;
	pthread_mutex_unlock(&coder->mutex);
	i = 0;
	while (i < coder->params->number_of_coders)
	{
		pthread_mutex_lock(&coder->params->coders->coders[i]->mutex);
		if (coder->params->coders->coders[i]->compiles > 0)
		{
			if (coder->params->coders->coders[i]->last_compile < last_compile)
			{
				pthread_mutex_unlock(&coder->params->coders->coders[i]->mutex);
				return (0);
			}
		}
		pthread_mutex_unlock(&coder->params->coders->coders[i]->mutex);
		i++;
	}
	return (1);
}

void	*ft_fifo(void *arg)
{
	t_coder			*coder;
	int				state;

	state = 1;
	coder = (t_coder *)arg;
	while (coder->compiles > 0 && state == 1)
	{
		pthread_mutex_lock(&coder->params->mutex);
		state = coder->params->state;
		pthread_mutex_unlock(&coder->params->mutex);
		if (take_dongles(coder) == 1)
			compile(coder);
		if (check_state(coder->params) == 1)
			usleep(coder->params->dongle_cooldown * 1000);
		unlock_dongles(coder);
		usleep(1000);
	}
	return (NULL);
}

void	*ft_edf(void *arg)
{
	t_coder			*coder;
	int				state;

	state = 1;
	coder = (t_coder *)arg;
	while (coder->compiles > 0 && state == 1)
	{
		pthread_mutex_lock(&coder->params->mutex);
		state = coder->params->state;
		pthread_mutex_unlock(&coder->params->mutex);
		if (ft_min(coder))
		{
			if (take_dongles(coder) == 1)
				compile(coder);
			if (check_state(coder->params) == 1)
				usleep(coder->params->dongle_cooldown * 1000);
			unlock_dongles(coder);
		}
		usleep(1000);
	}
	return (NULL);
}
