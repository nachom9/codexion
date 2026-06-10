/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imelero- <imelero-@student.42madrid.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 17:54:21 by imelero-          #+#    #+#             */
/*   Updated: 2026/06/10 17:54:23 by imelero-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	main(int argc, char *argv[])
{
	t_params	*params;
	t_coders	*coders;
	t_dongles	*dongles;
	int			i;
	int			params_checker;

	i = 0;
	if (argc != 9)
	{
		write(2, "Invalid number of arguments\n", 29);
		return (0);
	}
	params_checker = check_params(argv);
	if (params_checker > 0)
		return (print_error(params_checker));
	params = parse_params(argv);
	dongles = set_dongles(params->number_of_coders);
	params->dongles = dongles;
	coders = set_coders(params);
	params->coders = coders;
	set_threads(params, coders);
	return (0);
}
