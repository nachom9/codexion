/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imelero- <imelero-@student.42madrid.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 17:54:28 by imelero-          #+#    #+#             */
/*   Updated: 2026/06/10 17:54:29 by imelero-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
	node->time_to_burnout = ft_atol(args[2]);
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

int	print_error(int inv_arg)
{
	if (inv_arg == 1)
		write(2, "Error. Invalid number of coders\n", 33);
	else if (inv_arg == 2)
		write(2, "Error. Invalid time to burnout\n", 32);
	else if (inv_arg == 3)
		write(2, "Error. Invalid time to compile\n", 32);
	else if (inv_arg == 4)
		write(2, "Error. Invalid time to debug\n", 30);
	else if (inv_arg == 5)
		write(2, "Error. Invalid time to refactor\n", 33);
	else if (inv_arg == 6)
		write(2, "Error. Invalid compiles required\n", 34);
	else if (inv_arg == 7)
		write(2, "Error. Invalid dongle cooldown\n", 32);
	else if (inv_arg == 8)
		write(2, "Error. Invalid scheduler\n", 26);
	return (0);
}

int	check_params(char **args)
{
	int	i;
	int	len;

	i = 1;
	while (args[i + 1])
	{
		len = strlen(args[i]);
		if (ft_strisdigit(args[i]) == 1 && (len > 0 && len < 6))
			i++;
		else
			return (i);
	}
	if (strcmp(args[i], "fifo") && strcmp(args[i], "edf"))
		return (i);
	return (0);
}
