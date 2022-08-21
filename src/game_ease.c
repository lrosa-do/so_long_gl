/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_ease.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lrosa-do <lrosa-do@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/10 20:51:26 by lrosa-do          #+#    #+#             */
/*   Updated: 2022/05/28 16:33:03 by lrosa-do         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"
#include <math.h>

// t argument 0 = time since start of tween
// b argument 1 = beginning position
// c argument 2 = change in position
// d argument 3 = duration of tween in frames
double	ease_in_out_circle(double t, double b, double c, double d)
{
	double	vt;

	vt = t / (d / 2);
	if (vt < 1)
		return (c / 2 * (1 - sqrt(1 - vt * vt)) + b);
	vt -= 2;
	return (c / 2 * (sqrt(1 - vt * vt) + 1) + b);
}

double	ease_out_sin(double t, double b, double c, double d)
{
	return (c * sin(t / d * (FTPI / 2)) + b);
}

double	ease_in_sin(double t, double b, double c, double d)
{
	return (c * (1 - cos(t / d * (FTPI / 2))) + b);
}

double	ease_in_out_quad(double time, double b, double c, double d)
{
	double	t;

	t = time / (d / 2);
	if (t < 1)
		return (c / 2 * t * t + b);
	t -= 1;
	return (-c / 2 * ((t) * (t - 2) - 1) + b);
}

double	ease_in_back(double t, double b, double c, double d)
{
	double	s;
	double	post_fix;

	s = 1.70158f;
	t /= d;
	post_fix = t;
	return (c * (post_fix) * t * ((s + 1.0) * t - s) + b);
}
