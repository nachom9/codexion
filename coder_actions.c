/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_actions.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imelero- <imelero-@student.42madrid.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 17:53:54 by imelero-          #+#    #+#             */
/*   Updated: 2026/06/10 17:53:56 by imelero-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	unlock_dongles(t_coder	*coder)
{
	int	left;
	int	right;

	right = coder->id - 1;
	left = coder->id % coder->params->number_of_coders;
	pthread_mutex_unlock(&coder->params->dongles->dongles[left]->mutex);
	pthread_mutex_unlock(&coder->params->dongles->dongles[right]->mutex);
}

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
	printf("%ld %d has taken a dongle\n",
		get_time() - coder->params->start_time, coder->id);
	printf("%ld %d has taken a dongle\n",
		get_time() - coder->params->start_time, coder->id);
	return (1);
}

int	take_dongles_odd(t_coder *coder, int right, int left)
{
	pthread_mutex_lock(&coder->params->dongles->dongles[left]->mutex);
	if (check_state(coder->params) == 0)
	{
		pthread_mutex_unlock(&coder->params->dongles->dongles[left]->mutex);
		return (0);
	}
	pthread_mutex_lock(&coder->params->dongles->dongles[right]->mutex);
	printf("%ld %d has taken a dongle\n",
		get_time() - coder->params->start_time, coder->id);
	printf("%ld %d has taken a dongle\n",
		get_time() - coder->params->start_time, coder->id);
	return (1);
}

int	take_dongles(t_coder *coder)
{
	int	left;
	int	right;
	int	check;

	right = coder->id - 1;
	left = coder->id % coder->params->number_of_coders;
	if (coder->id % 2 == 0)
		check = take_dongles_even(coder, right, left);
	else
		check = take_dongles_odd(coder, right, left);
	if (check_state(coder->params) == 0 && check == 1)
	{
		pthread_mutex_unlock(&coder->params->dongles->dongles[left]->mutex);
		pthread_mutex_unlock(&coder->params->dongles->dongles[right]->mutex);
		return (0);
	}
	return (1);
}
