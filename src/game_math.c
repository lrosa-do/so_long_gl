/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_math.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lrosa-do <lrosa-do@student.42lisboa>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/10 20:51:26 by lrosa-do          #+#    #+#             */
/*   Updated: 2022/05/29 12:31:15 by lrosa-do         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"
#include <math.h>

int	ft_clamp(int value, int min, int max)
{
	if (max > min)
	{
		if (value < min)
			return (min);
		else if (value > max)
			return (max);
		else
			return (value);
	}
	else
	{
		if (value < max)
			return (max);
		else if (value > min)
			return (min);
		else
			return (value);
	}
}

int	ft_mod(int a, int b)
{
	int	ret;

	if (b < 0)
		return (-ft_mod(-a, -b));
	ret = a % b;
	if (ret < 0)
		ret += b;
	return (ret);
}

int	ft_cap(int value, int min, int max)
{
	return (min + ft_mod(value - min, max - min + 1));
}

float	ft_angle(t_gameobject *a, t_gameobject *b)
{
	int		delta_y;
	int		delta_x;
	float	angle;

	delta_y = b->y - a->y;
	delta_x = b->x - a->x;
	angle = atan2(delta_y, delta_x) * (180 / 3.141);
	return (angle);
}

double	ft_distance(double x1, double y1, double x2, double y2)
{
	return (sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1)));
}
