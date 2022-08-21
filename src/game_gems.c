/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_gems.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lrosa-do <lrosa-do@student.42lisboa>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/10 20:51:26 by lrosa-do          #+#    #+#             */
/*   Updated: 2022/05/29 20:03:22 by lrosa-do         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

static void	gem_handle_start(t_game *game, t_gameobject *gem)
{
	gem->start_x = gem->x;
	gem->start_y = gem->y;
	gem->end_x = game->width - 80;
	gem->end_y = 10;
	gem->tween_time = 0;
	gem->tween_steps = ft_distance(gem->x, gem->y, gem->end_x, gem->end_y) / 10;
	gem->vars[4] = 1;
}

static void	gem_handle_end(t_gameobject *gem)
{
	double	dst;

	dst = 0;
	gem->x = ease_in_back(gem->tween_time, gem->start_x, \
		(gem->end_x - gem->start_x), gem->tween_steps);
	gem->y = ease_in_back(gem->tween_time, gem->start_y, \
		(gem->end_y - gem->start_y), gem->tween_steps);
	if (gem->tween_time < gem->tween_steps)
		gem->tween_time += 1.5;
	dst = ft_distance(gem->x, gem->y, gem->end_x, gem->end_y);
	if (dst <= 30 || gem->y <= 25)
	{
		gem->live = 0;
		return ;
	}
}

static void	gem_handle_hit(t_game *game, t_gameobject *gem)
{
	if (gem->hit)
	{
		if (gem->vars[4] == 0)
		{
			gem_handle_start(game, gem);
		}
		else if (gem->vars[4] == 1)
		{
			gem_handle_end(gem);
		}
	}
}

static void	gem_update(void *g, t_gameobject *gem)
{
	t_game	*game;

	game = (t_game *)g;
	if (!gem || !game)
		return ;
	if (get_time(g) >= gem->timers[0] + gem->vars[3])
	{
		if (gem->vars[1] > 205)
			gem->vars[1] = 200;
		gem->vars[1]++;
		gem->timers[0] = get_time(g);
	}
	gem->graph = ft_cap(gem->vars[1], 200, 204);
	gem_handle_hit(game, gem);
}

t_gameobject	*create_collect(t_game *g, int x, int y, int graph)
{
	t_gameobject	*obj;

	obj = create_gameobject(g, x, y, graph);
	if (!obj)
		return (NULL);
	obj->vars[0] = 1;
	obj->vars[1] = irandom(200, 204);
	obj->vars[3] = irandom(90, 105);
	obj->vars[4] = 0;
	obj->timers[0]=0;
	obj->tween_time = 0;
	obj->tween_steps = 0;
	obj->type = GAME_COLLECT;
	obj->bound = c_rect(1, 1, 15, 15);
	obj->onupdate = &gem_update;
	return (obj);
}
