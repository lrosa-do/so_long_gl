/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_player_bullet.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lrosa-do <lrosa-do@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/10 20:51:26 by lrosa-do          #+#    #+#             */
/*   Updated: 2022/06/04 13:17:30 by lrosa-do         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

static void	bullet_update(void *g, t_gameobject *b)
{
	t_game			*game;
	t_gameobject	*obj_colider;

	game = (t_game *)g;
	if (!game || !b)
		return ;
	b->x -= BULLET_SPEED * b->vars[1];
	b->y -= BULLET_SPEED * b->vars[2];
	if (!game_objects_out_screen(game, b))
		b->live = 0;
	obj_colider = colide_type(game, b, GAME_SOLID);
	if (obj_colider)
	{
		b->live = 0;
		create_explosion(game, b->x - 20, b->y - 20);
		create_smoke_emitter(game, b->x, b->y, 0);
	}
}

static void	bullet_init_speed(t_gameobject *b, int x, int y)
{
	if (b->vars[0] == 4)
	{
		b->vars[1] = 0;
		b->vars[2] = 2;
		b->x = x + 12;
		b->y = y - 12;
	}
	else if (b->vars[0] == 3)
	{
		b->vars[1] = 2;
		b->vars[2] = 0;
		b->y = y + 18;
	}
	else if (b->vars[0] == 2)
	{
		b->vars[1] = -2;
		b->vars[2] = 0;
		b->x = x + 32;
		b->y = y + 36 / 2;
	}
}

t_gameobject	*create_player_bullet(t_game *g, int x, int y, int dir)
{
	t_gameobject	*obj;

	obj = create_gameobject(g, x, y, GAME_PLAYER_BULLET);
	if (!obj)
		return (NULL);
	obj->vars[0] = dir;
	obj->vars[1] = 0;
	obj->vars[2] = 0;
	obj->type = GAME_PLAYER_BULLET;
	obj->bound = c_rect(1, 1, 4, 4);
	bullet_init_speed(obj, x, y);
	if (dir == 1)
	{
		obj->vars[1] = 0;
		obj->vars[2] = -2;
		obj->x = x + 28 / 2;
		obj->y = y + 30;
	}
	obj->onupdate = &bullet_update;
	return (obj);
}
