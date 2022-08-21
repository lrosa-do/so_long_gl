/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_player.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lrosa-do <lrosa-do@student.42lisboa>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/10 20:51:26 by lrosa-do          #+#    #+#             */
/*   Updated: 2022/05/30 20:37:10 by lrosa-do         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"
#include <math.h>


static void	player_move_y(t_gameobject *player, t_game *game)
{
    int doBlur=0;
	if (game->keys[KEY_W] && place_free(game, player, player->x, player->y - 1))
	{
		player->y -= 16;
		game->moves_count++;
		player->vars[0] = 4;
		game->post->Blur=1;
		doBlur=1;
	}
	else if (game->keys[KEY_S]
		&& place_free(game, player, player->x, player->y + 15))
	{
		player->y += 16;
		game->moves_count++;
		player->vars[0] = 1;
		game->post->Blur=1;
		doBlur=1;
	}
	if (game->keys[KEY_A] && place_free(game, player, player->x - 1, player->y))
	{
		player->x -= 16;
		game->moves_count++;
		player->vars[0] = 3;
		game->post->Blur=1;
		doBlur=1;
	}
	else if (game->keys[KEY_D]
		&& place_free(game, player, player->x + 15, player->y))
	{
		player->x += 16;
		game->moves_count++;
		player->vars[0] = 2;
		game->post->Blur=1;
		doBlur=1;
	}
	if (get_time(game) >= player->timers[0] + 45)
	{
		if (player->vars[1] > 110)
			player->vars[1] = 100;
		player->vars[1]++;
		player->timers[0] = get_time(game);
		if (game->post->Blur==1)
	 {
	 game->post->Blur=0;

	}

	}


	if (player->vars[0] == 1)
		player->graph = ft_cap(player->vars[1], 100, 101);
	if (player->vars[0] == 2)
		player->graph = ft_cap(player->vars[1], 102, 103);
	if (player->vars[0] == 3)
		player->graph = ft_cap(player->vars[1], 104, 105);
	if (player->vars[0] == 4)
		player->graph = ft_cap(player->vars[1], 106, 107);
}

static void	player_move_x(t_gameobject *player, t_game *game)
{

}

static void	player_input(t_gameobject *player, t_game *game)
{
	t_gameobject	*obj_colider;

	player_move_y(player, game);
	player_move_x(player, game);
	obj_colider = colide_type(game, player, GAME_COLLECT);
	if (obj_colider)
	{
		obj_colider->hit = 1;
		obj_colider->collidable = 0;
	}

}

void	player_update_ex(t_gameobject *player, t_game *game)
{
	t_gameobject	*en_colider;

	player_input(player, game);
	if (game->keys[KEY_SHOOT] && (get_time(game) > player->timers[1] + 500))
	{
		player->timers[1] = get_time(game);
		create_player_bullet(game, player->x, player->y, player->vars[0]);
	}
	en_colider = colide_type(game, player, GAME_ENEMY);
	if (en_colider)
	{
		en_colider->energia = -1;
		player->hit = 1;
		player->timers[1] = get_time(game);
		create_explosion(game, player->x, player->y - 16);
		create_explosion(game, en_colider->x, en_colider->y - 16);
		return ;
	}
	if (player->energia < 30)
	{
	player->parts->visible = 1;
	player->parts->x = player->x;
	player->parts->y = player->y;
	game->post->Chaos=1;
	}
}


static void	player_collide_bullet(t_game *game, t_gameobject	*player)
{
	t_gameobject	*obj_colider;

	obj_colider = colide_type(game, player, GAME_ENEMY_BULLET);
	if (obj_colider)
	{
		obj_colider->live = 0;
		player->energia -= 5;

		create_explosion(game, player->x, player->y - 16);
	}
}


static void	player_draw(void *g, t_gameobject	*player)
{
	t_game			*game;

	game = (t_game *)g;
	if (!player || !game)
		return ;

}
static void	player_update(void *g, t_gameobject	*player)
{
	t_game			*game;

	game = (t_game *)g;
	if (!player || !game)
		return ;
	if (player->energia < 5)
	{
		player->hit = 1;
		create_explosion(game, player->x - 5, player->y - 16);
		create_explosion(game, player->x, player->y - 16);
	}
	if (player->hit)
	{
		if ((get_time(game) > player->timers[1] + 600))
		{
			game->game_state = 1;
			player->hit = 0;
		}
		return ;
	}
	player_update_ex(player, game);
	player_collide_bullet(game, player);

	 if (game->exit)
	{
		if (game->colected >= game->mapa->colect)
		{
			draw_rect(game, game->exit->rect, CGREEN);
			if (collide_with(player, game->exit, player->x, player->y))
			{
				player->active = 0;
				player->parts->active = 0;
				game->game_state = 2;
			}
		}
		else if (collide_with(player, game->exit, player->x, player->y))
		{
			draw_rect(game, game->exit->rect, CRED);
			//game->post->Chaos=1;
        } else
        {
           //if (game->post->Chaos==1)
          // {
           //game->post->Chaos=0;
         //  }

        }
	}
  ///  draw_rect(game, game->exit->rect, CRED);

}

t_gameobject	*create_player(t_game *g, int x, int y, int graph)
{
	t_gameobject	*player;

	player = create_gameobject_last(g, x, y, graph);
	if (!player)
		return (NULL);
	g->moves_count = 0;
	player->parts = create_smoke_emitter(g, x, y, 1);
	player->parts->visible = 0;
	player->vars[0] = 4;
	player->vars[1] = 100;
	player->timers[0] = get_time(g);
	player->timers[1] = get_time(g);
	player->type = GAME_PLAYER;
	player->collidable = 1;
	player->bound = c_rect(1, 1, 30, 30);
	player->onupdate = &player_update;

	return (player);
}
