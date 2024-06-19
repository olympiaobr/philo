/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olobresh <olobresh@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 17:50:30 by olobresh          #+#    #+#             */
/*   Updated: 2024/06/06 17:50:31 by olobresh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philosophers.h"

int	digits_only(int c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}

void	ft_putstr_fd(char *s, int fd)
{
	if (!s)
		return ;
	while (*s)
	{
		write(fd, s, 1);
		s++;
	}
}

void	ft_err(char *err)
{
	ft_putstr_fd(err, 2);
}

int	parse_sign(const char **str)
{
	int	sign;

	sign = 1;
	if (**str == '-' || **str == '+')
	{
		if (**str == '-')
		{
			sign = -1;
		}
		(*str)++;
	}
	return (sign);
}

int	custom_atoi(const char *str, int *result)
{
	long long	res;
	int			sign;

	res = 0;
	sign = parse_sign(&str);
	while (digits_only(*str))
	{
		res = res * 10 + (*str - '0');
		if (sign == 1 && res > INT_MAX)
		{
			*result = INT_MAX;
			return (0);
		}
		if (sign == -1 && -res < INT_MIN)
		{
			*result = INT_MIN;
			return (0);
		}
		str++;
	}
	*result = (int)(res * sign);
	return (1);
}
