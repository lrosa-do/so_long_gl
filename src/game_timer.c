/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_timer.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lrosa-do <lrosa-do@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/10 20:51:26 by lrosa-do          #+#    #+#             */
/*   Updated: 2022/06/04 16:15:21 by lrosa-do         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"
#include <sys/time.h>
#include <time.h>

int	get_time_now(void)
{
	int				t;
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	t = (int)(tv.tv_sec + tv.tv_usec / 1e6);
	return (t);
}

void	init_timer(t_game *g)
{
	srand((unsigned int)time(NULL));
	g->time_offset = get_timer_value();
	g->time_elapse = get_time(g);
	g->delay_elapse = get_time(g);
	g->last_time = 0;
	g->count_fps = 0;
	g->scree_fps = 60;
	g->tick_per_frame = 1000 / g->scree_fps;
}

void	updade_timer(t_game *g)
{
	int	frame_ticks;
	int	tick_time;

	g->delay_elapse = get_time(g);
	g->time = get_time(g);
	g->fps_avg = g->count_fps / ((get_time(g) - g->time_elapse) / 1000.f);
	if (g->fps_avg > 2000000)
		g->fps_avg = 0;
	frame_ticks = (get_time(g) - g->delay_elapse);
	//if (frame_ticks < g->tick_per_frame)
	//	ft_sleep(g->tick_per_frame - frame_ticks);
	++g->count_fps;
	tick_time = get_time(g);
	g->deltatime = (tick_time - g->last_time) / 1000.0f ;
	if (g->deltatime >= 1.0f)
		g->deltatime = 1.0f;
	g->last_time = tick_time;
}

void	ft_sleep(float ms)
{
	/*struct timespec	req;
	time_t			sec;

	bzero(&req, 0);
	sec = (int)(ms / 1000.0f);
	ms -= (sec * 1000);
	req.tv_sec = sec;
	req.tv_nsec = ms * 1000000L;
	while (nanosleep(&req, &req) == -1)
		continue ;*/
}
