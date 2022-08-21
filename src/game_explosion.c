/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_explosion.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lrosa-do <lrosa-do@student.42lisboa>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/10 20:51:26 by lrosa-do          #+#    #+#             */
/*   Updated: 2022/05/29 20:23:01 by lrosa-do         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

static void	explosion_update(void *g, t_gameobject *b)
{
	t_game	*game;

	game = (t_game *)g;
	if (!b || !game)
		return ;
	if (get_time(game) >= b->timers[0] + b->vars[1])
	{
		b->timers[0] = get_time(g);
		b->graph++;
		game->shake = 1;
		//game->post->Shake =1;
		game->shake_timer = get_time(game);
	}
	if (b->graph > 463)
		b->live = 0;
}

t_gameobject	*create_explosion(t_game *g, int x, int y)
{
	t_gameobject	*obj;

	obj = create_gameobject_last(g, x, y, 450);
	if (!obj)
		return (NULL);
	obj->timers[0] = get_time(g);
	obj->y -= g->off_set;
	obj->vars[0] = 0;
	obj->vars[1] = 22;
	obj->type = GAME_EXPLOSION;
	obj->bound = c_rect(1, 1, 15, 15);
	obj->onupdate = &explosion_update;
	shake_screen(g);
	return (obj);
}
