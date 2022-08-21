/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_emitter.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lrosa-do <lrosa-do@student.42lisboa>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/30 18:11:26 by lrosa-do          #+#    #+#             */
/*   Updated: 2022/05/30 19:02:07 by lrosa-do         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

t_parengine	*game_emitter_create(void)
{
	t_parengine	*lib;

	lib = (t_parengine *)mem_malloc(sizeof(t_parengine));
	if (!lib)
		return (NULL);
	lib->count = 0;
	lib->root = NULL;
	return (lib);
}

void	game_emitter_destroy(t_parengine *lib)
{
	t_emitter	*current;
	t_emitter	*next;

	if (!lib)
		return ;
	if (lib->root)
	{
		current = lib->root;
		while (current != NULL)
		{
			next = current->next;
			current->onupdate = NULL;
			if (current->particles)
				mem_free(current->particles);
			current->particles = NULL;
			mem_free(current);
			current = next;
		}
		lib->root = NULL;
	}
	mem_free(lib);
	lib = NULL;
}

void	game_emitter_update(t_game *g)
{
	t_emitter	*current;

	if (!g->gamepar || !g->gamepar->root)
		return ;
	current = g->gamepar->root;
	while (current)
	{
		if (current->onupdate)
			if (current->active && current->visible)
				current->onupdate(g, current);
		current = current->next;
	}
}

void	game_emitter_clean(t_game *g)
{
	t_emitter	*current;
	t_emitter	*previous;

	if (!g->gamepar || !g->gamepar->root)
		return ;
	current = g->gamepar->root;
	while (current->active)
	{
		if (current->next == NULL)
			return ;
		else
		{
			previous = current;
			current = current->next;
		}
	}
	if (current == g->gamepar->root)
		g->gamepar->root = g->gamepar->root->next;
	else
		previous->next = current->next;
	if (current->particles)
		free(current->particles);
	free (current);
	g->gamepar->count -= 1;
	current = NULL;
}

int	game_emitter_add(t_parengine *lib, t_emitter *obj)
{
	t_emitter	*tmp;

	if (!lib)
		return (0);
	if (!lib->root)
	{
		lib->root = obj;
		obj->next = NULL;
	}
	else
	{
		tmp = lib->root;
		lib->root = obj;
		obj->next = tmp;
	}
	lib->count++;
	return (lib->count - 1);
}
