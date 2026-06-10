/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_dongles.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imelero- <imelero-@student.42madrid.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 17:54:42 by imelero-          #+#    #+#             */
/*   Updated: 2026/06/10 17:54:44 by imelero-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static t_dongle	*create_dongle(int id)
{
	t_dongle	*node;

	node = malloc(sizeof(t_dongle));
	if (!node)
		return (NULL);
	node->id = id;
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
