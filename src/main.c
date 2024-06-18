/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olobresh <olobresh@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 17:50:10 by olobresh          #+#    #+#             */
/*   Updated: 2024/06/06 17:50:12 by olobresh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philosophers.h"

//this function takes argc and argv as arguments and checks if the arguments are valid
int check_args(int argc, char **argv)
{
    int temp;
    int i;

    if (argc != 5 && argc != 6)
    {
        ft_err("Error: The program requires 4 or 5 arguments.\n");
        return (0);
    }
    i = 1;
    while (i < argc)
    {
        if (!check_int(argv[i], &temp) || temp < 0)
        {
            ft_err("Error: All arguments must be positive integers.\n");
            return (0);
        }
        i++;
    }
    if (!check_int(argv[1], &temp) || temp < 1 || temp > 200)
    {
        ft_err("Error: The number of philosophers must be between 1 and 200.\n");
        return (0);
    }
    return (1);
}


int check_parameters(t_info *info)
{
    if (info->ms_to_starve < 60 || info->ms_to_eat < 60 || info->ms_to_sleep < 60)
    {
        ft_err("Error: Time to die, eat, and sleep must be at least 60 ms.\n");
        return 0;
    }
    if (info->times_eating < 0 && info->times_eating != -1)
    {
        ft_err("Error: Number of times each philosopher must eat must be at least 0.\n");
        return 0;
    }
    return 1;
}


int main(int argc, char **argv)
{
	t_info	info;

	if (!check_args(argc, argv))
	{
		return (1);
	}
	begin_philosophy(&info, argc, argv);
	if (!check_parameters(&info))
    {
        return (1);
    }
	if (start_philosopher_threads(&info))
	{
		free_info(&info, "Error starting philosopher threads.\n", EXIT_FAILURE);

	}
	supervision(&info);
	if (thread_termination(&info)) {
        free_info(&info, "Error terminating philosopher threads.\n", EXIT_FAILURE);
    }
    cleanup(&info);
	return (0);
}

