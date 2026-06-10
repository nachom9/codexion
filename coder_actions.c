#include "codexion.h"

int	compile(t_coder *coder)
{
	if (check_state(coder->params) == 0)
		return (0);
	printf("%ld %d is compiling\n",
		get_time() - coder->params->start_time, coder->id);
	coder->last_compile = get_time() - coder->params->start_time;
	usleep(coder->params->time_to_compile * 1000);
	if (check_state(coder->params) == 0)
		return (0);
	printf("%ld %d is debugging\n",
		get_time() - coder->params->start_time, coder->id);
	usleep(coder->params->time_to_debug * 1000);
	if (check_state(coder->params) == 0)
		return (0);
	printf("%ld %d is refactoring\n",
		get_time() - coder->params->start_time, coder->id);
	usleep(coder->params->time_to_refactor * 1000);
	coder->compiles -= 1;
	return (0);
}

int	take_dongles_even(t_coder *coder, int right, int left)
{
	pthread_mutex_lock(&coder->params->dongles->dongles[right]->mutex);
	if (check_state(coder->params) == 0)
	{
		pthread_mutex_unlock(&coder->params->dongles->dongles[right]->mutex);
		return (0);
	}
	pthread_mutex_lock(&coder->params->dongles->dongles[left]->mutex);
	return (0);
}

int	take_dongles_even(t_coder *coder, int right, int left)
{
	pthread_mutex_lock(&coder->params->dongles->dongles[left]->mutex);
	if (check_state(coder->params) == 0)
	{
		pthread_mutex_unlock(&coder->params->dongles->dongles[left]->mutex);
		return (0);
	}
	pthread_mutex_lock(&coder->params->dongles->dongles[right]->mutex);
	return (0);
}

int	take_dongles(t_coder *coder)
{
	int	left;
	int	right;

	right = coder->id - 1;
	left = coder->id % coder->params->number_of_coders;
	if (coder->id % 2 == 0)
		take_dongles_even(coder, right, left);
	else
		take_dongles_odd(coder, right, left);
	if (check_state(coder->params) == 0)
	{
		pthread_mutex_unlock(&coder->params->dongles->dongles[left]->mutex);
		pthread_mutex_unlock(&coder->params->dongles->dongles[right]->mutex);
		return (0);
	}
	printf("%ld %d has taken a dongle\n",
		get_time() - coder->params->start_time, coder->id);
	printf("%ld %d has taken a dongle\n",
		get_time() - coder->params->start_time, coder->id);
	compile(coder);
	if (check_state(coder->params) == 1)
		usleep(coder->params->dongle_cooldown * 1000);
	pthread_mutex_unlock(&coder->params->dongles->dongles[left]->mutex);
	pthread_mutex_unlock(&coder->params->dongles->dongles[right]->mutex);
	return (0);
}
