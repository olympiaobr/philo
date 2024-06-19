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

void	take_forks(t_philo *philo)
{
	pthread_mutex_t	*first_fork;
	pthread_mutex_t	*second_fork;

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
		handle_single_philo(philo, first_fork);
		return ;
	}
	pthread_mutex_lock(second_fork);
	print_message(philo, "has taken a fork");
}

void	check_full(t_philo *philo)
{
	pthread_mutex_lock(&philo->info->mut_full);
	if (philo->info->full_philos >= philo->info->nbr_philo)
	{
		pthread_mutex_unlock(&philo->info->mut_full);
		return ;
	}
	pthread_mutex_unlock(&philo->info->mut_full);
}

void	eat_action(t_philo *philo)
{
	long long	current_time;

	print_message(philo, "is eating");
	current_time = get_the_time();
	pthread_mutex_lock(philo->m);
	philo->meal_prev = current_time;
	philo->meal_count++;
	pthread_mutex_unlock(philo->m);
	usleep(philo->eat_t * 1000);
}

void	philo_eat(t_philo *philo)
{
	check_full(philo);
	take_forks(philo);
	if (philo->info->nbr_philo == 1)
	{
		return ;
	}
	eat_action(philo);
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
	if (philo->meal_count >= philo->info->times_eating
		&& philo->info->times_eating != -1)
	{
		pthread_mutex_lock(&philo->info->mut_full);
		philo->info->full_philos++;
		pthread_mutex_unlock(&philo->info->mut_full);
	}
}

void	*philo_activities(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->id % 2 == 0)
	{
		usleep(10000);
	}
	while (1)
	{
		check_conditions(philo);
		philo_eat(philo);
		if (philo->info->nbr_philo == 1)
		{
			break ;
		}
		philo_sleep(philo);
		philo_think(philo);
	}
	return (NULL);
}
