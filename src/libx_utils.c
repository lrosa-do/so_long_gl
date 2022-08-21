/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libx_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lrosa-do <lrosa-do@student.42lisboa>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/10 20:51:26 by lrosa-do          #+#    #+#             */
/*   Updated: 2022/05/29 11:47:16 by lrosa-do         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"



unsigned int	mlx_get_color(int a, int r, int g, int b)
{
	return (a << 24 | r << 16 | g << 8 | b);
}

t_bitmap	*mlx_create_bitmap(void *mlx, int width, int height)
{
	t_bitmap	*bitmap;


	return (bitmap);
}

t_bitmap	*mlx_load_bitmap(void *mlx, char *filename)
{
	t_bitmap	*bitmap;



	return (bitmap);
}

void	mlx_destroy_bitmap(t_game *g, t_bitmap *bitmap)
{
	if (!bitmap)
		return ;

}

void	mlx_draw_bitmap(t_game *g, t_bitmap *gr, int x, int y)
{
	unsigned int	color;
	int				i;
	int				j;


}

t_rect	c_rect(int x, int y, int w, int h)
{
	t_rect	r;

	r.x = x;
	r.y = y;
	r.w = w;
	r.h = h;
	return (r);
}

t_point	c_point(int x, int y)
{
	t_point	c;

	c.x = x;
	c.y = y;
	return (c);
}

t_color	c_color(int r, int g, int b, int a)
{
	t_color	c;

	c.r = r;
	c.g = g;
	c.b = b;
	c.a = a;
	return (c);
}

