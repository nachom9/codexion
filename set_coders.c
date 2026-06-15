/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_coders.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imelero- <imelero-@student.42madrid.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 17:54:35 by imelero-          #+#    #+#             */
/*   Updated: 2026/06/10 17:54:37 by imelero-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static t_coder	*create_coder(int id, t_params *params)
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

void	set_threads(t_params *params, t_coders *coders)
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
	while (i < params->number_of_coders && check_state(params) == 1)
	{
		if (scheduler == 1)
			pthread_create(&coders->coders[i]->thread,
				NULL, ft_fifo, coders->coders[i]);
		else if (scheduler == 2)
			pthread_create(&coders->coders[i]->thread,
				NULL, ft_edf, coders->coders[i]);
		i++;
	}
	pthread_join(params->death_thread, NULL);
	free_all(params);
}
