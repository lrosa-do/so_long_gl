/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_enemy_bullet.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lrosa-do <lrosa-do@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/10 20:51:26 by lrosa-do          #+#    #+#             */
/*   Updated: 2022/06/04 13:15:50 by lrosa-do         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

static int	get_offset(int index)
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

static void	enemy_bullet_update(void *g, t_gameobject *b)
{
	t_game			*game;
	int				angle ;
	t_gameobject	*obj_colider;

	game = (t_game *)g;
	if (!game || !b)
		return ;
	angle = get_offset(b->vars[0]);
	b->x -= cosf(angle * FTPI / 180.0f) * BULLET_SPEED;
	b->y -= sinf(angle * FTPI / 180.0f) * BULLET_SPEED;
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

t_gameobject	*create_enemy_bullet(t_game *g, int x, int y, int dir)
{
	t_gameobject	*obj;

	obj = create_gameobject(g, x, y, GAME_ENEMY_BULLET);
	if (!obj)
		return (NULL);
	obj->vars[0] = dir;
	obj->y -= g->off_set;
	obj->type = GAME_ENEMY_BULLET;
	obj->bound = c_rect(1, 1, 4, 4);
	obj->onupdate = &enemy_bullet_update;
	return (obj);
}
