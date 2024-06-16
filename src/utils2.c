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

void	custom_usleep(long long time_in_ms)
{
	long long	start_time;
	long long	current_t;

	start_time = get_the_time();
	while ((current_t = get_the_time()) - start_time < time_in_ms)
		usleep(100);
}
int	death_check(t_philo *philo)
{
	pthread_mutex_lock(&philo->info->mut_dead);
	if (philo->info->dead)
	{
		pthread_mutex_unlock(&philo->info->mut_dead);
		return (1);
	}
	pthread_mutex_unlock(&philo->info->mut_dead);
	return (0);
}

void print_message(t_philo *philo, char *message)
{
    long long timestamp;

    if (death_check(philo))
        return;
    pthread_mutex_lock(&philo->info->mut_write); // Lock before reading the start time
    timestamp = time_diff(philo->info->start, NULL); // Use info->start for consistency
    pthread_mutex_unlock(&philo->info->mut_write); // Unlock after reading the start time

    pthread_mutex_lock(&philo->info->print);
    printf("%lld %d %s\n", timestamp, philo->id, message);
    pthread_mutex_unlock(&philo->info->print);
}
