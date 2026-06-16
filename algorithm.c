/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   algorithm.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imelero- <imelero-@student.42madrid.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 18:33:31 by imelero-          #+#    #+#             */
/*   Updated: 2026/06/10 18:33:32 by imelero-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	check_dongles(t_coder *coder)
{
	int		left;
	int		right;
	long	time;

	left = coder->id - 1;
	right = coder->id % coder->params->number_of_coders;
	pthread_mutex_lock(&coder->params->dongles->dongles[left]->mutex);
	pthread_mutex_lock(&coder->params->dongles->dongles[right]->mutex);
	time = get_time();
	if (time >= coder->params->dongles->dongles[left]->cooldown
		&& time >= coder->params->dongles->dongles[right]->cooldown)
	{
		pthread_mutex_unlock(&coder->params->dongles->dongles[left]->mutex);
		pthread_mutex_unlock(&coder->params->dongles->dongles[right]->mutex);
		return (1);
	}
	pthread_mutex_unlock(&coder->params->dongles->dongles[left]->mutex);
	pthread_mutex_unlock(&coder->params->dongles->dongles[right]->mutex);
	return (0);
}

static int	ft_min(t_coder *coder)
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
	int				compile_check;

	state = 1;
	compile_check = 1;
	coder = (t_coder *)arg;
	while (coder->compiles > 0 && state == 1)
	{
		pthread_mutex_lock(&coder->params->mutex);
		state = coder->params->state;
		pthread_mutex_unlock(&coder->params->mutex);
		if (check_dongles(coder) == 1)
		{
			if (take_dongles(coder) == 1)
				compile_check = compile(coder);
		}
		if (compile_check == 2)
		{
			unlock_dongles(coder);
		}
		usleep(1000);
	}
	return (NULL);
}

void	*ft_edf(void *arg)
{
	t_coder			*coder;
	int				state;
	int				compile_check;

	state = 1;
	coder = (t_coder *)arg;
	compile_check = 1;
	while (coder->compiles > 0 && state == 1)
	{
		pthread_mutex_lock(&coder->params->mutex);
		state = coder->params->state;
		pthread_mutex_unlock(&coder->params->mutex);
		if (ft_min(coder))
		{
			if (check_dongles(coder) == 1)
			{
				if (take_dongles(coder) == 1)
					compile_check = compile(coder);
			}
			if (compile_check == 2)
				unlock_dongles(coder);
		}
		usleep(1000);
	}
	return (NULL);
}
