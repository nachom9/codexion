/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imelero- <imelero-@student.42madrid.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 17:55:02 by imelero-          #+#    #+#             */
/*   Updated: 2026/06/10 17:55:03 by imelero-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	ft_strisdigit(char	*str)
{
	int	i;

	i = 0;
	if (!str || !str[0])
		return (0);
	while (str[i])
	{
		if (str[i] >= '0' && str[i] <= '9')
		{
			i++;
		}
		else
			return (0);
	}
	return (1);
}

long	ft_atol(const char *str)
{
	long	result;

	result = 0;
	while (*str >= '0' && *str <= '9')
	{
		result = result * 10 + (*str - '0');
		str++;
	}
	return (result);
}

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
