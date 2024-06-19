/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olobresh <olobresh@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 18:08:58 by olobresh          #+#    #+#             */
/*   Updated: 2024/06/06 18:08:59 by olobresh         ###   ########.fr       */
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

void takes_forks(t_philo *philo)
{
    pthread_mutex_t *first_fork;
    pthread_mutex_t *second_fork;

    if (philo->id % 2 == 0)
    {
        first_fork = philo->left_f;
        second_fork = philo->right_f;
    }
    else
    {
        first_fork = philo->right_f;
        second_fork = philo->left_f;
    }

    pthread_mutex_lock(first_fork);
    print_message(philo, "has taken a fork");
    if (philo->info->nbr_philo == 1)
    {
        usleep(philo->die_t * 1000);
        pthread_mutex_unlock(first_fork);
        print_message(philo, "died");
        pthread_mutex_lock(&philo->info->mut_dead);
        philo->info->dead = 1;
        pthread_mutex_unlock(&philo->info->mut_dead);
        return;
    }
    pthread_mutex_lock(second_fork);
    print_message(philo, "has taken a fork");
}

void philo_eat(t_philo *philo)
{
    long long current_time;

    pthread_mutex_lock(&philo->info->mut_full);
    if (philo->info->full_philos >= philo->info->nbr_philo)
    {
        pthread_mutex_unlock(&philo->info->mut_full);
        return;
    }
    pthread_mutex_unlock(&philo->info->mut_full);

    takes_forks(philo);
    if (philo->info->nbr_philo == 1)
    {
        return;
    }
    print_message(philo, "is eating");
    current_time = get_the_time();
    pthread_mutex_lock(philo->m);
    philo->meal_prev = current_time;
    philo->meal_count++;
    pthread_mutex_unlock(philo->m);
    usleep(philo->eat_t * 1000);

    if (philo->id % 2 == 0)
    {
        pthread_mutex_unlock(philo->left_f);
        pthread_mutex_unlock(philo->right_f);
    }
    else
    {
        pthread_mutex_unlock(philo->right_f);
        pthread_mutex_unlock(philo->left_f);
    }

    if (philo->meal_count >= philo->info->times_eating && philo->info->times_eating != -1)
    {
        pthread_mutex_lock(&philo->info->mut_full);
        philo->info->full_philos++;
        pthread_mutex_unlock(&philo->info->mut_full);
    }
}

void *philo_activities(void *arg)
{
    t_philo *philo;

    philo = (t_philo *)arg;
    if (philo->id % 2 == 0)
    {
        usleep(10000);
    }
    while (1)
    {
        pthread_mutex_lock(&philo->info->mut_dead);
        if (philo->info->dead)
        {
            pthread_mutex_unlock(&philo->info->mut_dead);
            break;
        }
        pthread_mutex_unlock(&philo->info->mut_dead);
        pthread_mutex_lock(&philo->info->mut_full);
        if (philo->info->full_philos >= philo->info->nbr_philo)
        {
            pthread_mutex_unlock(&philo->info->mut_full);
            break;
        }
        pthread_mutex_unlock(&philo->info->mut_full);
        philo_eat(philo);
        if (philo->info->nbr_philo == 1)
        {
            break;
        }
        philo_sleep(philo);
        philo_think(philo);
    }
    return NULL;
}

