/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_objects_add.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lrosa-do <lrosa-do@student.42lisboa>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/10 20:51:26 by lrosa-do          #+#    #+#             */
/*   Updated: 2022/05/29 20:07:55 by lrosa-do         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

int	game_objects_add(t_gameengine *lib, t_gameobject *obj)
{
	t_gameobject	*tmp;

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

int	game_objects_add_back(t_gameengine *lib, t_gameobject *obj)
{
	t_gameobject	*current;

	if (!lib)
		return (0);
	if (!lib->root)
	{
		lib->root = obj;
		obj->next = NULL;
	}
	else
	{
		current = lib->root;
		while (current->next != NULL)
			current = current->next;
		current->next = obj;
		current->next->next = NULL;
	}
	lib->count++;
	return (lib->count - 1);
}

t_gameobject	*create_gameobject(t_game *g, int x, int y, int graph)
{
	t_gameobject	*obj;


	obj = (t_gameobject *)mem_malloc(sizeof(t_gameobject));
	if (!obj)
		return (NULL);
	obj->x = x;
	obj->y = y + g->off_set;
	obj->energia = 100;
	obj->live = 1;
	obj->active = 1;
	obj->collidable = 1;
	obj->type = graph;
	obj->graph = graph;
	obj->hit = 0;
	obj->onupdate = NULL;
	obj->onrender = NULL;
	Graph *gr = GetAssetGraph(g->assets,graph);
	if (gr)
    	obj->bound = c_rect(1, 1, gr->width, gr->height);
    else
        obj->bound = c_rect(0, 0, 1, 1);


	game_objects_add(g->gameobjects, obj);
	return (obj);
}

t_gameobject	*create_gameobject_last(t_game *g, int x, int y, int graph)
{
	t_gameobject	*obj;


	obj = (t_gameobject *)mem_malloc(sizeof(t_gameobject));
	if (!obj)
		return (NULL);
	obj->x = x;
	obj->y = y + g->off_set;
	obj->energia = 100;
	obj->live = 1;
	obj->active = 1;
	obj->collidable = 1;
	obj->type = graph;
	obj->graph = graph;
	obj->hit = 0;
	obj->onupdate = NULL;
	obj->onrender = NULL;
	obj->parts = NULL;
	Graph *gr = GetAssetGraph(g->assets,graph);
	if (gr)
    	obj->bound = c_rect(1, 1, gr->width, gr->height);
    else
        obj->bound = c_rect(0, 0, 1, 1);
	game_objects_add_back(g->gameobjects, obj);
	return (obj);
}
