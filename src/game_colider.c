/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_colider.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lrosa-do <lrosa-do@student.42lisboa>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/10 20:51:26 by lrosa-do          #+#    #+#             */
/*   Updated: 2022/05/01 07:57:52 by lrosa-do         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

int	collide_with(t_gameobject *a, t_gameobject *e, int x, int y)
{
	int	_x;
	int	_y;

	_x = a->x;
	_y = a->y;
	a->x = x;
	a->y = y;
	if (a && e)
	{
		if (e->collidable && e->live && e->active
			&& x - a->bound.x + a->bound.w > e->x - e->bound.x
			&& y - a->bound.y + a->bound.h > e->y - e->bound.y
			&& x - a->bound.x < e->x - e->bound.x + e->bound.w
			&& y - a->bound.y < e->y - e->bound.y + e->bound.h)
		{
			a->x = _x;
			a->y = _y;
			return (1);
		}
	}
	a->x = _x;
	a->y = _y;
	return (0);
}

t_gameobject	*place_meeting(t_game *g, t_gameobject *a, int x, int y)
{
	t_gameobject	*current;

	if (g->gameobjects && g->gameobjects->root)
	{
		current = g->gameobjects->root;
		while (current)
		{
			if (collide_with(a, current, x, y))
				return (current);
			current = current->next;
		}
	}
	return (NULL);
}

int	place_empty(t_game *g, t_gameobject *a, int x, int y)
{
	t_gameobject	*current;

	if (g->gameobjects && g->gameobjects->root)
	{
		current = g->gameobjects->root;
		while (current)
		{
			if (current->live && collide_with(a, current, x, y))
				return (0);
			current = current->next;
		}
	}
	return (1);
}

int	place_free(t_game *g, t_gameobject *a, int x, int y)
{
	t_gameobject	*current;

	if (g->gameobjects && g->gameobjects->root)
	{
		current = g->gameobjects->root;
		while (current)
		{
			if (collide_with(a, current, x, y) && current->type == GAME_SOLID)
				return (0);
			current = current->next;
		}
	}
	return (1);
}

t_gameobject	*colide_type(t_game *g, t_gameobject *a, int type)
{
	t_gameobject	*current;

	if (g->gameobjects && g->gameobjects->root)
	{
		current = g->gameobjects->root;
		while (current)
		{
			if (collide_with(a, current, a->x, a->y) && current->type == type)
				return (current);
			current = current->next;
		}
	}
	return (NULL);
}


int	line_in_line(t_rect l, t_rect r)
{
	double	ua;
	double	ub;

	ua = (double)((r.w - r.x) * (l.y - r.y) - (r.h - r.y) * (l.x - r.x)) / \
	(double)((r.h - r.y) * (l.w - l.x) - (r.w - r.x) * (l.h - l.y));
	ub = (double)((l.w - l.x) * (l.y - r.y) - (l.h - l.y) * (l.x - r.x)) / \
	(double)((r.h - r.y) * (l.w - l.x) - (r.w - r.x) * (l.h - l.y));
	if (ua >= 0 && ua <= 1 && ub >= 0 && ub <= 1)
		return (1);
	return (0);
}

int	line_in_rect(t_rect l, t_rect r)
{
	int	left;
	int	right;
	int	top;
	int	bottom;

	left = line_in_line(c_rect(l.x, l.y, l.w, l.h), \
	c_rect(r.x, r.y, r.x, r.y + r.h));
	right = line_in_line(c_rect(l.x, l.y, l.w, l.h), \
	c_rect(r.x + r.w, r.y, r.x + r.w, r.y + r.h));
	top = line_in_line(c_rect(l.x, l.y, l.w, l.h), \
	c_rect(r.x, r.y, r.x + r.w, r.y));
	bottom = line_in_line(c_rect(l.x, l.y, l.w, l.h), \
	c_rect(r.x, r.y + r.h, r.x + r.w, r.y + r.h));
	return (left || right || top || bottom);
}

int	rect_contains_point(t_rect r, t_point p)
{
	return (r.x <= p.x && p.x < r.x + r.w && r.y <= p.y && p.y < r.y + r.h);
}

int	line_intersects(t_point l1p1, t_point l1p2, t_point l2p1, t_point l2p2)
{
	float	d;
	float	q;
	float	r;
	float	s;

	q = (float)(l1p1.y - l2p1.y)*(l2p2.x - l2p1.x) - \
	(l1p1.x - l2p1.x) * (l2p2.y - l2p1.y);
	d = (float)(l1p2.x - l1p1.x)*(l2p2.y - l2p1.y) - \
	(l1p2.y - l1p1.y) * (l2p2.x - l2p1.x);
	if (d == 0)
		return (0);
	r = q / d;
	q = (l1p1.y - l2p1.y) * (l1p2.x - l1p1.x) - \
	(l1p1.x - l2p1.x) * (l1p2.y - l1p1.y);
	s = q / d;
	if (r < 0 || r > 1 || s < 0 || s > 1)
	{
		return (0);
	}
	return (1);
}

int	linrect(t_point p1, t_point p2, t_rect r)
{
	return (line_intersects(p1, p2, c_point(r.x, r.y), \
	c_point(r.x + r.w, r.y)) || line_intersects(p1, p2, \
	c_point(r.x + r.w, r.y), \
	c_point(r.x + r.w, r.y + r.h)) || line_intersects(p1, p2, \
	c_point(r.x + r.w, r.y + r.h), \
	c_point(r.x, r.y + r.h)) || line_intersects(p1, p2, \
	c_point(r.x, r.y + r.h), \
	c_point(r.x, r.y)) || (rect_contains_point(r, p1) \
	&& rect_contains_point(r, p2)));
}
