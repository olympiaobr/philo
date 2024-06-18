/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olobresh <olobresh@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 17:49:53 by olobresh          #+#    #+#             */
/*   Updated: 2024/06/06 17:49:55 by olobresh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <stdio.h>
# include <stdbool.h>
# include <string.h>
# include <stdlib.h>
# include <unistd.h>
#include <limits.h>
# include <pthread.h>
# include <time.h>
# include <sys/time.h>
# include <stdint.h>

typedef struct s_info t_info;

typedef enum s_action
{
	EATING,
	SLEEPING,
	THINKING,
	FORK_ONE,
	FORK_TWO,
	DIED,
}	t_action;

typedef struct s_philo {
    int id;
    long long meal_prev;
    int meal_count;
    int die_t;
    int eat_t;
    int sleep_t;
    int left_fork_id;
    int right_fork_id;
    pthread_mutex_t *left_f;
    pthread_mutex_t *right_f;
    pthread_mutex_t *m;
    pthread_t id_thread; // Added this line
    struct s_info *info;
} t_philo;

typedef struct s_info {
    int nbr_philo;
    int ms_to_starve;
    int ms_to_eat;
    int ms_to_sleep;
    int times_eating;
    int continue_sim;
    int dead;
    int ate;
    int full_philos;
    long long start;
    pthread_t tracker;
    pthread_mutex_t *status;
    pthread_mutex_t print;
    pthread_mutex_t mut_dead;
    pthread_mutex_t mut_meal;
    pthread_mutex_t mut_write;
    pthread_mutex_t mut_full;
    pthread_mutex_t *continue_mut;
    pthread_mutex_t *meal_count_mutex;
    t_philo **philos;
} t_info;

int check_args(int argc, char **argv);
int check_parameters(t_info *info);

void	init_philosophers(t_info *info);
void	init_forks(t_info *info);
void init_mutexes(t_info *info);
void begin_philosophy(t_info *info, int argc, char **argv);

/*	actions	*/
void	philo_sleep(t_philo *philo);
void	philo_think(t_philo *philo);
void	philo_eat(t_philo *philo);
void	*philo_activities(void *arg);
int	start_philosopher_threads(t_info *info);
int	thread_termination(t_info *info);
void	cleanup(t_info *info);

int	dead(t_philo *philo);
int	all_philos_fed(t_info *info);
int	track_fullness(t_info *info);
int	track_death(t_info *info);
void	*philo_supervision(void *arg);
void	supervision(t_info *info);
int main(int argc, char **argv);

/*	utils	*/
int check_int(const char *str, int *result);
void	ft_err(char *err);
void	custom_usleep(long long time_in_ms);
int	digits_only(int c);
void	ft_putstr_fd(char *s, int fd);
size_t	ft_strlen(const char *s);
long long	get_the_time(void);
long long	time_diff(long long start_time, t_info *info);
int	death_check(t_philo *philo);
void	print_message(t_philo *philo, char *message);
void free_info(t_info *info, char *err_msg, int status);

#endif

