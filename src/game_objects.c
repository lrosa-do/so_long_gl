/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_objects.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lrosa-do <lrosa-do@student.42lisboa>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/10 20:51:26 by lrosa-do          #+#    #+#             */
/*   Updated: 2022/05/29 20:14:39 by lrosa-do         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

t_gameengine	*game_objects_create(void)
{
	t_gameengine	*lib;

	lib = (t_gameengine *)mem_malloc(sizeof(t_gameengine));
	if (!lib)
		return (NULL);
	lib->count = 0;
	lib->root = NULL;
	return (lib);
}

void	game_objects_destroy(t_gameengine *lib)
{
	t_gameobject	*current;
	t_gameobject	*next;

	if (!lib)
		return ;
	if (lib->root)
	{
		current = lib->root;
		while (current != NULL)
		{
			next = current->next;
			current->onupdate = NULL;
			current->onrender = NULL;
			mem_free(current);
			current = next;
		}
		lib->root = NULL;
	}
	mem_free(lib);
	lib = NULL;
}

void	game_objects_render(t_game *g)
{
	t_gameobject	*current;

	if (!g->gameobjects || !g->gameobjects->root)
		return ;
	current = g->gameobjects->root;
	while (current)
	{
		draw_alpha_sprite(
			g, current->graph, current->x - g->p.x, current->y - g->p.y);
		current->rect.x = current->x;
		current->rect.y = current->y;
		current->rect.w = current->bound.w;
		current->rect.h = current->bound.h;
		if (current->onrender)
			if (current->active)
				current->onrender(g, current);

		current = current->next;
	}
	game_objects_clean(g);
}



void	game_objects_update(t_game *g)
{
	t_gameobject	*current;

	if (!g->gameobjects || !g->gameobjects->root)
		return ;
	current = g->gameobjects->root;
	while (current)
	{
		if (current->onupdate)
			if (current->active)
				current->onupdate(g, current);
		current = current->next;
	}
	game_objects_render(g);
}
