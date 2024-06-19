/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olobresh <olobresh@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/19 14:58:39 by olobresh          #+#    #+#             */
/*   Updated: 2024/06/19 14:58:41 by olobresh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philosophers.h"

void philo_sleep(t_philo *philo)
{
    print_message(philo, "is sleeping");
    usleep(philo->sleep_t * 1000);
    pthread_mutex_lock(&philo->info->mut_dead);
    if (!philo->info->dead && dead(philo))
	{
        philo->info->dead = 1;
        pthread_mutex_unlock(&philo->info->mut_dead);
        print_message(philo, "died");
        return;
    }
    pthread_mutex_unlock(&philo->info->mut_dead);
}

void philo_think(t_philo *philo)
{
    print_message(philo, "is thinking");
	usleep(1000);
    pthread_mutex_lock(&philo->info->mut_dead);
    if (!philo->info->dead && dead(philo))
	{
        philo->info->dead = 1;
        pthread_mutex_unlock(&philo->info->mut_dead);
        print_message(philo, "died");
        return;
    }
    pthread_mutex_unlock(&philo->info->mut_dead);
}
void handle_single_philo(t_philo *philo, pthread_mutex_t *first_fork)
{
    usleep(philo->die_t * 1000);
    pthread_mutex_unlock(first_fork);
    print_message(philo, "died");
    pthread_mutex_lock(&philo->info->mut_dead);
    philo->info->dead = 1;
    pthread_mutex_unlock(&philo->info->mut_dead);
}
void check_conditions(t_philo *philo)
{
    pthread_mutex_lock(&philo->info->mut_dead);
    if (philo->info->dead)
    {
        pthread_mutex_unlock(&philo->info->mut_dead);
        pthread_exit(NULL);
    }
    pthread_mutex_unlock(&philo->info->mut_dead);

    pthread_mutex_lock(&philo->info->mut_full);
    if (philo->info->full_philos >= philo->info->nbr_philo)
    {
        pthread_mutex_unlock(&philo->info->mut_full);
        pthread_exit(NULL);
    }
    pthread_mutex_unlock(&philo->info->mut_full);
}
