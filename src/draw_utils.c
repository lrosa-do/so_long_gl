/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lrosa-do <lrosa-do@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/10 20:51:26 by lrosa-do          #+#    #+#             */
/*   Updated: 2022/06/04 09:23:25 by lrosa-do         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"
#include <math.h>



void	draw_sprite(t_game *g, int id, int x, int y)
{
//SDL_Log("sprite %d ",id);
  RenderAssetTexture(g->assets,g->render,x,y,id,WHITE);
}


void	draw_alpha_sprite(t_game *g, int id, int x, int y)
{

//RenderAssetTexture(g->assets,g->render,x,y,id,WHITE);

Rect clip= GetAssetsAtlasClipById(g->assets,id);
RenderTextureClip(g->render,g->assets->TextureAtlas.texture,x,y,clip.width,clip.height,clip,WHITE);

}

void				draw_alpha_scale_sprite(t_game *g,int w,int h, int id, int x, int y)
{

Rect clip= GetAssetsAtlasClipById(g->assets,id);
RenderTextureClip(g->render,g->assets->TextureAtlas.texture,x,y,w,h,clip,WHITE);
//RenderTextureClip(g->render,g->assets->TextureAtlas.texture,x-1,y-1,clip.width+1,clip.height+1,clip,WHITE);
}


void	clear_bitmap(t_bitmap *g, int color)
{

}

void	draw_solid_rect(t_game *g, t_rect r, int hexValue)
{
  Color color;

    color.a = (unsigned char)(hexValue >> 24) & 0xFF;
    color.r = (unsigned char)(hexValue >> 16) & 0xFF;
    color.g = (unsigned char)(hexValue >> 8) & 0xFF;
    color.b = (unsigned char)hexValue & 0xFF;

    RenderSolidRectangle(g->lines,r.x,r.y,r.w,r.h,color);
}

void	draw_rect(t_game *g, t_rect r, int hexValue)
{
  Color color;

    color.a = (unsigned char)(hexValue >> 24) & 0xFF;
    color.r = (unsigned char)(hexValue >> 16) & 0xFF;
    color.g = (unsigned char)(hexValue >> 8) & 0xFF;
    color.b = (unsigned char)hexValue & 0xFF;

    RenderRectangle(g->lines,r.x,r.y,r.x+r.w,r.y+r.h,color);
}
