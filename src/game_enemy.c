/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_enemy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lrosa-do <lrosa-do@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/10 20:51:26 by lrosa-do          #+#    #+#             */
/*   Updated: 2022/06/04 16:14:57 by lrosa-do         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"
#include <math.h>


static void	enemy_update2(t_game *game, t_gameobject *b)
{
	if (b->hit)
	{
		draw_rect(game, c_rect(b->x + 2, b->y + 30, 25, 5), CBLACK);
		if (get_time(game) >= b->timers[0] + b->vars[2])
			b->hit = 0;
		if (b->energia > 50)
			draw_solid_rect(game, c_rect(b->x + 2, b->y + 30, b->energia / 5, 5), CGREEN);
		else
		{
			draw_solid_rect(game, c_rect(b->x + 2, b->y + 30, b->energia / 5, 5), CRED);
			b->parts->visible = 1;
		}
	}
	if (b->energia <= 0)
	{
		create_explosion(game, b->x - irandom(-8, 12), b->y - 25);
		b->active = 0;
		b->parts->active = 0;
		b->graph = GAME_ENEMY - 1;
	}
}

static void	enemy_update_ai(t_game *game, t_gameobject *b)
{
	int		dist;

	dist = (int)ft_distance(game->player->x, game->player->y, b->x, b->y);

//SDL_Log("%d %d %d %d",dist,b->vars[7],b->vars[5],b->timers[5]);

	if (dist <= (game->width / 2) && (b->vars[7] < 10))
	{

		if (b->vars[5] > 0)
		{
			if ((get_time(game) > b->timers[5] + 800))
			{
				b->timers[5] = get_time(game);
				b->vars[5] -= 1;
				enemy_shot_player(game, b);
			}
		}
	}
	if (b->vars[5] <= 0)
	{
		if ((get_time(game) > b->timers[5] + 2500))
		{
				b->timers[5] = get_time(game);
				b->vars[5] = 10;
		}
	}
}

void	enemy_update(void *g, t_gameobject *b)
{
	t_game			*game;
	t_gameobject	*obj_colider;

	game = (t_game *)g;
	if (!game || !b)
		return ;
	obj_colider = colide_type(game, b, GAME_PLAYER_BULLET);
	if (obj_colider)
	{
		obj_colider->live = 0;
		b->energia -= 25;
		b->hit = 1;
		b->timers[0] = get_time(g);
	}
	b->vars[7] = (int)abs(abs(b->vars[1]) - abs(angle_offset(b->vars[6])));

	enemy_update_ai(game, b);
}


int	angle_offset(int index)
{
	if (index == 300)
		return (0);
	if (index == 301)
		return (40);
	if (index == 302)
		return (90);
	if (index == 303)
		return (130);
	if (index == 304)
		return (180);
	if (index == 305)
		return (-140);
	if (index == 306)
		return (-90);
	if (index == 307)
		return (-40);
	return (0);
}

void	enemy_shot_player(t_game *game, t_gameobject *b)
{
	int	dir;

	dir = angle_offset(b->vars[6]);

	//SDL_Log("shot %d ",dir);
	b->vars[3] = b->x + 15 - cosf(dir * FTPI / 180.0f) * 30;
	b->vars[4] = b->y + 12 - sinf(dir * FTPI / 180.0f) * 30;
	create_enemy_bullet(game, b->vars[3], b->vars[4], b->vars[6]);
}


static int	get_xgraph(int index)
{
	int	graph[32];

	graph[0] = 300;
	graph[1] = 301;
	graph[2] = 301;
	graph[3] = 301;
	graph[4] = 302;
	graph[5] = 302;
	graph[6] = 303;
	graph[7] = 303;
	graph[8] = 303;
	graph[9] = 304;
	graph[10] = 304;
	graph[11] = 304;
	graph[12] = 305;
	graph[13] = 305;
	graph[14] = 306;
	graph[15] = 306;
	graph[16] = 306;
	graph[17] = 307;
	graph[18] = 307;
	graph[19] = 307;
	graph[20] = 300;
	graph[21] = 300;
	return (graph[index]);
}

static t_point	get_offset(int index)
{
	if (index == 0)
		return (c_point(7, 3));
	if (index == 1)
		return (c_point(6, 5));
	if (index == 2)
		return (c_point(-5, 6));
	if (index == 3)
		return (c_point(-1, 5));
	if (index == 4)
		return (c_point(-4, -4));
	if (index == 5)
		return (c_point(-2, 2));
	if (index == 6)
		return (c_point(-5, -5));
	if (index == 7)
		return (c_point(7, 0));
	return (c_point(0, 0));
}

static t_point	get_size(int index)
{
	if (index == 0)
		return (c_point(7, 3));
	if (index == 1)
		return (c_point(6, 5));
	if (index == 2)
		return (c_point(-5, 6));
	if (index == 3)
		return (c_point(-1, 5));
	if (index == 4)
		return (c_point(-4, -4));
	if (index == 5)
		return (c_point(-2, 2));
	if (index == 6)
		return (c_point(-5, -5));
	if (index == 7)
		return (c_point(7, 0));
	return (c_point(0, 0));
}

static	void	enemy_render2(t_game *game, t_gameobject *b)
{
	int		xgraph;
	int		index;
	int		a;
	t_point	pt;

	a = b->vars[1] % 360;
	if (a < 0)
		a += 360;
	index = (int)(a * 21 / 360);
	xgraph = get_xgraph(index);
	pt = get_offset(xgraph - 300);
	b->vars[1] = (int)ft_angle(game->player, b);
	b->vars[6] = xgraph;
	b->vars[3] = b->x + 15 - cosf(b->vars[1] * FTPI / 180.0f) * 30;
	b->vars[4] = b->y + 12 - sinf(b->vars[1] * FTPI / 180.0f) * 30;
	draw_alpha_sprite(game, xgraph, (b->x - game->p.x) - pt.x, (b->y - game->p.y) - pt.y);
}

static	void	enemy_render(void *g, t_gameobject *b)
{
	t_game	*game;

	game = (t_game *)g;
	if (!game || !b)
		return ;
	enemy_render2(game, b);
	enemy_update2(game, b);

}


t_gameobject	*create_enemy(t_game *g, int x, int y)
{
	t_gameobject	*obj;

	obj = create_gameobject(g, x, y, GAME_ENEMY);
	if (!obj)
		return (NULL);
	obj->vars[0] = 0;
	obj->vars[1] = irandom(0, 360);
	obj->vars[2] = 100;
	obj->vars[5] = 100;
	obj->timers[0] = 0;
	obj->timers[1] = 0;
	obj->timers[5] = 0;
	obj->type = GAME_ENEMY;
	obj->parts = create_smoke_emitter(g, x + 10, y + 30, 1);
	obj->parts->visible = 0;
	obj->bound = c_rect(0, 0, 30, 30);
	obj->onupdate = &enemy_update;
	obj->onrender = &enemy_render;
	return (obj);
}
