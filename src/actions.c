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
    custom_usleep(philo->sleep_t);

    // Immediate death check after sleeping
    if (dead(philo))
    {
        print_message(philo, "died");
        pthread_mutex_lock(&philo->info->mut_dead);
        philo->info->dead = 1;
        pthread_mutex_unlock(&philo->info->mut_dead);
    }
}

void philo_think(t_philo *philo)
{
    print_message(philo, "is thinking");

    // Immediate death check after thinking
    if (dead(philo))
    {
        print_message(philo, "died");
        pthread_mutex_lock(&philo->info->mut_dead);
        philo->info->dead = 1;
        pthread_mutex_unlock(&philo->info->mut_dead);
    }
}

void philo_eat(t_philo *philo)
{
    long long current_time;
    pthread_mutex_t *first_fork;
    pthread_mutex_t *second_fork;

    // Determine the order of forks to avoid deadlocks
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
        custom_usleep(philo->die_t);
        pthread_mutex_unlock(first_fork);
        print_message(philo, "died");
        philo->info->dead = 1; // Mark the simulation as finished
        return;
    }
    pthread_mutex_lock(second_fork);
    print_message(philo, "has taken a fork");
    print_message(philo, "is eating");

    current_time = get_the_time();
    pthread_mutex_lock(philo->m);
    philo->meal_prev = current_time;
    philo->meal_count++;
    pthread_mutex_unlock(philo->m);

    custom_usleep(philo->eat_t);

    pthread_mutex_unlock(second_fork);
    pthread_mutex_unlock(first_fork);

    // Immediate death check after eating
    if (dead(philo))
    {
        print_message(philo, "died");
        pthread_mutex_lock(&philo->info->mut_dead);
        philo->info->dead = 1;
        pthread_mutex_unlock(&philo->info->mut_dead);
    }
}

void *philo_activities(void *arg)
{
    t_philo *philo;

    philo = (t_philo *)arg;
    if (philo->id % 2 == 0)
        custom_usleep(50);
    while (!death_check(philo) && !philo->info->dead) // Check for global death condition
    {
        philo_eat(philo);
        if (death_check(philo) || philo->info->dead)
            break;
        philo_sleep(philo);
        if (death_check(philo) || philo->info->dead)
            break;
        philo_think(philo);
        if (philo->meal_count >= philo->info->times_eating)
            break; // Stop if the philosopher has eaten enough times
    }
    return NULL;
}
