/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olobresh <olobresh@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 16:11:19 by olobresh          #+#    #+#             */
/*   Updated: 2024/06/10 16:11:20 by olobresh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philosophers.h"

int	start_philosopher_threads(t_info *info)
{
	int	i;

	pthread_mutex_lock(&info->mut_write);
    info->start = get_the_time();
    pthread_mutex_unlock(&info->mut_write);
	i = 0;
	while (i < info->nbr_philo)
	{
		if (pthread_create(&info->philos[i]->id_thread, NULL, philo_activities, (void *)info->philos[i]) != 0)
		{
			printf("Failed to create thread for philosopher %d\n", info->philos[i]->id);
			return (EXIT_FAILURE);
		}
		i++;
	}
	pthread_mutex_lock(info->continue_mut);
	info->continue_sim = 1;
	pthread_mutex_unlock(info->continue_mut);
	return (EXIT_SUCCESS);
}

int	thread_termination(t_info *info)
{
	int	i;

	if (pthread_join(info->tracker, NULL) != 0)
	{
		printf("Error: Could not join surveillance thread.\n");
		return (EXIT_FAILURE);
	}
	i = 0;
	while (i < info->nbr_philo)
	{
		if (pthread_join(info->philos[i]->id_thread, NULL) != 0)
		{
			printf("Error: Could not join thread for philosopher %d\n", info->philos[i]->id);
			return (EXIT_FAILURE);
		}
		i++;
	}
	return (EXIT_SUCCESS);
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
void cleanup_philosophers(t_info *info)
{
    int index;

    index = 0;
    while (index < info->nbr_philo)
    {
        if (info->philos[index])
        {
            if (info->philos[index]->m)
                pthread_mutex_destroy(info->philos[index]->m);
            free(info->philos[index]->m);
            free(info->philos[index]);
        }
        index++;
    }
}

void cleanup(t_info *info)
{
    int index;

    if (!info)
        return;
    cleanup_philosophers(info);
    pthread_mutex_destroy(&info->print);
    pthread_mutex_destroy(&info->mut_dead);
    pthread_mutex_destroy(&info->mut_meal);
    pthread_mutex_destroy(&info->mut_write);
    pthread_mutex_destroy(&info->mut_full);
    pthread_mutex_destroy(info->continue_mut);
    pthread_mutex_destroy(info->meal_count_mutex);
    free(info->continue_mut);
    free(info->meal_count_mutex);
    index = 0;
    while (index < info->nbr_philo)
    {
        pthread_mutex_destroy(&info->status[index]);
        index++;
    }
    free(info->status);
    free(info->philos);
}

