/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   func_extra.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lrosa-do <lrosa-do@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/10 20:51:26 by lrosa-do          #+#    #+#             */
/*   Updated: 2022/06/04 17:05:18 by lrosa-do         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"
#include <math.h>
#include <sys/time.h>
#include <time.h>


int	ft_strpos(const char *s, int c)
{
	int	i;

	i = 0;
	while (s[i] != '\0' && s[i] != (char) c)
		i++;
	if (s[i] == (char) c)
		return (i);
	return (0);
}

const char	*ft_format_text(const char *text, ...)
{
	static char	buffers[4][1024];
	static int	index;
	char		*c_buffer;
	va_list		args;

	index = 0;
	c_buffer = buffers[index];
	memset(c_buffer, 0, 1024);
	va_start(args, text);
	vsnprintf(c_buffer, 1024, text, args);
	va_end(args);
	index += 1;
	if (index >= 4)
		index = 0;
	return (c_buffer);
}

int	irandom(int min, int max)
{
	int		tmp;
	double	result;

	if (min > max)
	{
		tmp = max;
		max = min;
		min = tmp;
	}
	result = (double) (rand() % (abs(max - min) + 1) + min);
	return ((int) result);
}

float	frandom(float min, float max)
{
	float	rn;

	rn = sin(rand() * rand());
	return (min + (max - min) * fabs(rn));
}

uint64_t	get_timer_value(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((uint64_t)tv.tv_sec * (uint64_t)1000000 + (uint64_t)tv.tv_usec);
}

int	get_time(t_game *g)
{
	return ((int)(get_timer_value() - g->time_offset) / 1000);
}
