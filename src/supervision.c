/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   supervision.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olobresh <olobresh@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 14:46:47 by olobresh          #+#    #+#             */
/*   Updated: 2024/06/12 14:46:48 by olobresh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philosophers.h"

int	dead(t_philo *philo)
{
	long long	time_since_last_meal;
	int			not_eating;

	pthread_mutex_lock(philo->m);
	time_since_last_meal = get_the_time() - philo->meal_prev;
	not_eating = (time_since_last_meal > philo->die_t);
	pthread_mutex_unlock(philo->m);
	return (not_eating);
}

int	all_philos_fed(t_info *info)
{
	int	i;
	int	fed_philos;

	i = 0;
	fed_philos = 0;
	if (info->times_eating == -1)
		return (0);
	while (i < info->nbr_philo)
	{
		pthread_mutex_lock(info->philos[i]->m);
		if (info->philos[i]->meal_count >= info->times_eating)
			fed_philos++;
		pthread_mutex_unlock(info->philos[i]->m);
		i++;
	}
	return (fed_philos == info->nbr_philo);
}

int	track_fullness(t_info *info)
{
	if (all_philos_fed(info))
	{
		pthread_mutex_lock(&info->mut_dead);
		if (info->dead == 0)
		{
			info->dead = 1;
			pthread_mutex_unlock(&info->mut_dead);
			pthread_mutex_lock(&info->print);
			printf("All philosophers have eaten %d times\n",
				info->times_eating);
			pthread_mutex_unlock(&info->print);
			info->continue_sim = 0;
			return (1);
		}
		pthread_mutex_unlock(&info->mut_dead);
	}
	return (0);
}

int	track_death(t_info *info)
{
	int	i;

	i = 0;
	while (i < info->nbr_philo)
	{
		if (dead(info->philos[i]))
		{
			pthread_mutex_lock(&info->mut_dead);
			if (info->dead == 0)
			{
				info->dead = 1;
				pthread_mutex_unlock(&info->mut_dead);
				print_message(info->philos[i], "died");
				info->continue_sim = 0;
				return (1);
			}
			pthread_mutex_unlock(&info->mut_dead);
		}
		i++;
	}
	return (0);
}

void	*philo_supervision(void *arg)
{
	t_info	*info;

	info = (t_info *)arg;
	while (1)
	{
		pthread_mutex_lock(&info->mut_dead);
		if (info->dead)
		{
			pthread_mutex_unlock(&info->mut_dead);
			break ;
		}
		pthread_mutex_unlock(&info->mut_dead);
		if (track_fullness(info))
			break ;
		if (track_death(info))
			break ;
		custom_usleep(100);
	}
	return (NULL);
}

void	supervision(t_info *info)
{
	pthread_create(&info->tracker, NULL, &philo_supervision, (void *)info);
}
