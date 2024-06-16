/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialize.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olobresh <olobresh@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 18:08:47 by olobresh          #+#    #+#             */
/*   Updated: 2024/06/06 18:08:49 by olobresh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philosophers.h"

void	init_philosophers(t_info *info)
{
    int i = 0;
    while (i < info->nbr_philo)
    {
        info->philos[i] = (t_philo *)malloc(sizeof(t_philo));
        if (!info->philos[i])
        {
            free_info(info, "Error: Memory allocation failed for philosopher.\n", EXIT_FAILURE);
        }
        info->philos[i]->id = i + 1;
        info->philos[i]->meal_prev = get_the_time();
        info->philos[i]->meal_count = 0;
        info->philos[i]->die_t = info->ms_to_starve;
        info->philos[i]->eat_t = info->ms_to_eat;
        info->philos[i]->sleep_t = info->ms_to_sleep;
        info->philos[i]->right_f = &info->status[i];
        info->philos[i]->left_f = &info->status[(i + 1) % info->nbr_philo];
        info->philos[i]->m = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
        if (!info->philos[i]->m || pthread_mutex_init(info->philos[i]->m, NULL) != 0)
        {
            free_info(info, "Error: Mutex initialization failed for philosopher.\n", EXIT_FAILURE);
        }
        info->philos[i]->info = info;
        i++;
    }
}

void	init_forks(t_info *info)
{
	int	i;

	i = 0;
	while (i < info->nbr_philo)
	{
		if (pthread_mutex_init(&info->status[i], NULL) != 0)
		{
			free_info(info, "Error: Mutex initialization failed for fork.\n", EXIT_FAILURE);
		}
		i++;
	}
}

void free_info(t_info *info, char *err_msg, int status)
{
    if (err_msg)
        ft_putstr_fd(err_msg, 2);
    if (info)
    {
        cleanup(info);
        free(info);
    }
    exit(status);
}
void init_mutexes(t_info *info)
{
	if (pthread_mutex_init(&info->print, NULL) != 0 ||
		pthread_mutex_init(&info->mut_dead, NULL) != 0 ||
		pthread_mutex_init(&info->mut_meal, NULL) != 0 ||
		pthread_mutex_init(&info->mut_write, NULL) != 0)
	{
		free_info(info, "Error: Mutex initialization failed.\n", EXIT_FAILURE);

	}
	info->continue_mut = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
	info->meal_count_mutex = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
	if (!info->continue_mut || !info->meal_count_mutex)
	{
		free_info(info, "Error: Memory allocation failed for mutexes.\n", EXIT_FAILURE);
	}
	if (pthread_mutex_init(info->continue_mut, NULL) != 0 ||
		pthread_mutex_init(info->meal_count_mutex, NULL) != 0)
	{
		free_info(info, "Error: Mutex initialization failed.\n", EXIT_FAILURE);
	}
}

void begin_philosophy(t_info *info, int argc, char **argv)
{
	if (!check_int(argv[1], &info->nbr_philo) ||
        !check_int(argv[2], &info->ms_to_starve) ||
        !check_int(argv[3], &info->ms_to_eat) ||
        !check_int(argv[4], &info->ms_to_sleep))
    {
        free_info(info, "Error: Invalid arguments.\n", EXIT_FAILURE);

    }
	if (argc == 6)
    {
        if (!check_int(argv[5], &info->times_eating))
        {
            ft_err("Error: Invalid fifth argument.\n");
            exit(EXIT_FAILURE);
        }
    }
	else
    {
        info->times_eating = -1;
    }
	info->continue_sim = 1;
	info->dead = 0;
	info->ate = 0;
	init_mutexes(info);
	info->philos = (t_philo **)malloc(sizeof(t_philo *) * info->nbr_philo);
	info->status = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * info->nbr_philo);
	if (!info->philos || !info->status)
	{
        free_info(info, "Error: Memory allocation failed for philosopher or status mutexes.\n", EXIT_FAILURE);

	}
	init_forks(info);
	init_philosophers(info);
}

