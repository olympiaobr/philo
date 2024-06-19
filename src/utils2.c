/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olobresh <olobresh@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 17:50:36 by olobresh          #+#    #+#             */
/*   Updated: 2024/06/06 17:50:38 by olobresh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philosophers.h"

long long	get_the_time(void)
{
	struct timeval	t;
	long long		curr_t;

	if (gettimeofday(&t, NULL) != 0)
	{
		ft_err("gettimeofday failed\n");
		return (-1);
	}
	curr_t = (t.tv_sec * 1000) + (t.tv_usec / 1000);
	return (curr_t);
}

long long	time_diff(long long start_time, t_info *info)
{
	long long	current_t;

	current_t = get_the_time();
	if (current_t == -1)
		return (-1);
	if (start_time == 0 && info != NULL)
		start_time = info->start;
	return (current_t - start_time);
}

int	death_check(t_philo *philo)
{
	int	is_dead;

	pthread_mutex_lock(&philo->info->mut_dead);
	is_dead = philo->info->dead;
	pthread_mutex_unlock(&philo->info->mut_dead);
	return (is_dead);
}

void print_message(t_philo *philo, char *message)
{
    long long timestamp;

    pthread_mutex_lock(&philo->info->print);
    pthread_mutex_lock(&philo->info->mut_dead);
    if (philo->info->dead && strcmp(message, "died") != 0)
	{
        pthread_mutex_unlock(&philo->info->mut_dead);
        pthread_mutex_unlock(&philo->info->print);
        return;
    }
    timestamp = time_diff(philo->info->start, NULL);
    printf("%lld %d %s\n", timestamp, philo->id, message);
    pthread_mutex_unlock(&philo->info->mut_dead);
    pthread_mutex_unlock(&philo->info->print);
}
