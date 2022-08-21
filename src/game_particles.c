/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_particles.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lrosa-do <lrosa-do@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/30 18:11:26 by lrosa-do          #+#    #+#             */
/*   Updated: 2022/06/04 16:14:06 by lrosa-do         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

void	smoke_update(float dt, t_particle *particle)
{
		particle->x -= particle->vx * dt * 20.5f;
		particle->y -= particle->vy * dt * 20.5f;
		particle->vy -= 0.1f * dt * 3.0f;
		particle->live -= 0.1f * dt * 5.0f;
}
void	particle_init(t_particle *particle, int x, int y)
{
		particle->x = x;
		particle->y = y;
		particle->vx = 0;
		particle->vy = 0;
		particle->live = 1;
		particle->active = 1;
}

void	smoke_reset(t_emitter *emitter, t_particle *particle)
{
		particle->x = emitter->x + irandom(4, 12);
		particle->y = emitter->y + irandom(5, 10);
		particle->vx = frandom(-0.45f, 0.45f);
		particle->vy = frandom(0.4f, 0.8f);
		particle->live = 1;
		particle->active = 1;
}

void	smoke_update_oneshot(t_game *g, t_emitter *emitter)
{
	int	index;

	if (!emitter->loop)
	{
		emitter->live += 0.9f * g->deltatime * 1.5f;
		if (emitter->live >= emitter->max_live)
		{
			emitter->active = 0;
			return ;
		}
	}
	index = 0;
	if ((get_time(g) > emitter->timer + 90))
	{
		emitter->timer = get_time(g);
		while (index < MAX_PARTICLES)
		{
			if (emitter->particles[index].live <= 0)
			{
				smoke_reset(emitter, &emitter->particles[index]);
				break ;
			}
			index++;
		}
	}
}

void	smoke_update_emitter(void *g, t_emitter *emitter)
{
	t_game	*game;
	int		index;

	game = (t_game *)g;
	if (!emitter->visible)
		return ;
	smoke_update_oneshot(game, emitter);
	index = 0;
	while (index < MAX_PARTICLES)
	{
		if (emitter->particles[index].active)
		{
			smoke_update(game->deltatime, &emitter->particles[index]);
			draw_solid_rect(game, c_rect((int)emitter->particles[index].x, \
				(int)emitter->particles[index].y, 2, 2), CWHITE);
		}
		index ++;
	}
}

t_emitter	*create_smoke_emitter(t_game *g, int x, int y, int loop)
{
	t_emitter	*obj;
	int			index;

	index = 0;
	obj = (t_emitter *)mem_malloc(sizeof(t_emitter));
	if (!obj)
		return (NULL);
	obj->x = x;
	obj->y = y;
	obj->active = 1;
	obj->visible = 1;
	obj->loop = loop;
	obj->live = 0;
	obj->max_live = 1.0f;
	obj->particles = (t_particle *) mem_malloc(sizeof(t_particle) * (MAX_PARTICLES + 1));
	while (index < MAX_PARTICLES)
	{
		particle_init(&obj->particles[index], x, y);
		index++;
	}
	obj->onupdate = &smoke_update_emitter;
	game_emitter_add(g->gamepar, obj);
	return (obj);
}
