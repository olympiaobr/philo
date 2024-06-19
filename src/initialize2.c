/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialize2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olobresh <olobresh@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/19 16:06:23 by olobresh          #+#    #+#             */
/*   Updated: 2024/06/19 16:06:24 by olobresh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philosophers.h"

void	begin_philosophy(t_info *info, int argc, char **argv)
{
	process_args(info, argc, argv);
	info->continue_sim = 1;
	info->dead = 0;
	info->ate = 0;
	info->full_philos = 0;
	init_mutexes(info);
	info->philos = (t_philo **)malloc(sizeof(t_philo *) * info->nbr_philo);
	if (!info->philos || !info->status)
	{
		free_info(info,
			"Error: Memory allocation failed for philos or status mutexes.\n",
			EXIT_FAILURE);
	}
	init_forks(info);
	init_philosophers(info);
}
