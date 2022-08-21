/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   so_long.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lrosa-do <lrosa-do@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/25 07:41:13 by lrosa-do          #+#    #+#             */
/*   Updated: 2022/06/04 15:42:50 by lrosa-do         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SO_LONG_H
# define SO_LONG_H


# include "Engine.h"
# include <stdio.h>
# include <fcntl.h>
# include <stdlib.h>
# include <math.h>

# define KEY_ESC 27
# define KEY_SHOOT 32
# define KEY_Q 113
# define KEY_W 119
# define KEY_A 97
# define KEY_S 115
# define KEY_D 100
# define TILE_WIDTH 32
# define TILE_HEIGHT 32
# define CRED   0xFFFF0000
# define CGREEN 0xFF00FF00
# define CBLUE  0xFF0000FF
# define CWHITE 0xFFFFFFFF
# define CBLACK 0xFF000000
# define BULLET_SPEED    5
# define MAX_PARTICLES   25

# define GAME_PLAYER  100
# define GAME_COLLECT 4
# define GAME_ENEMY   400
# define GAME_SOLID   1
# define GAME_START   20
# define GAME_EXIT   21
# define GAME_EXPLOSION 450
# define GAME_PLAYER_BULLET   500
# define GAME_ENEMY_BULLET    501

# define FTPI 3.14159265358979323846f

# ifndef TIMER_STEP
#  define TIMER_STEP 10
# endif

typedef struct s_color
{
	int		r;
	int		g;
	int		b;
	int		a;
}t_color;

typedef struct s_point
{
	int		x;
	int		y;
}t_point;

typedef struct s_rect
{
	int		x;
	int		y;
	int		w;
	int		h;
}t_rect;

typedef struct s_bitmap
{
	void			*buffer;
	char			*addr;
	int				bpp;
	int				line_len;
	int				endian;
	int				width;
	int				height;
	t_color			ck;
}t_bitmap;

typedef struct s_graph
{
	int				width;
	int				height;
	Texture2D		image;
	int				id;
	struct s_graph	*next;
}t_graph;

typedef struct s_particle
{
	float	x;
	float	y;
	float	vx;
	float	vy;
	float	live;
	int		active;
}t_particle;

typedef struct s_emitter
{
	int					x;
	int					y;
	int					active;
	int					visible;
	int					loop;
	float				live;
	float				max_live;
	float				timer;
	t_particle			*particles;
	void				(*onupdate)(void	*game, struct s_emitter *obj);
	struct s_emitter	*next;
}t_emitter;

typedef struct s_gameobject
{
	int					x;
	int					y;
	int					start_x;
	int					start_y;
	int					end_x;
	int					end_y;
	double				tween_time;
	double				tween_steps;
	t_rect				bound;
	t_rect				rect;
	int					energia;
	int					graph;
	int					update;
	int					live;
	int					active;
	int					hit;
	int					collidable;
	int					type;
	int					timers[10];
	int					vars[10];
	t_emitter			*parts;
	void				(*onupdate)(void	*game, struct s_gameobject *obj);
	void				(*onrender)(void	*game, struct s_gameobject *obj);

	struct s_gameobject	*next;
}t_gameobject;

typedef struct s_parengine
{
	t_emitter	*root;
	int			count;
}t_parengine;

typedef struct s_gameengine
{
	t_gameobject	*root;
	int				count;
}t_gameengine;

typedef struct s_font
{
	int				cols;
	int				rows;
	int				w;
	int				h;
	char			*alpha;
	Graph		*image;
	Texture2D   texture;
}t_font;

typedef struct s_glib
{
	t_graph	*root;
	int		count;
}t_glib;

typedef struct s_map
{
	int		width;
	int		height;
	char    **map;
	int		colect;
	int		player;
	int		exit;
	int		player_x;
	int		player_y;
}	t_map;

typedef struct s_game
{
	

	t_gameengine	*gameobjects;
	t_parengine		*gamepar;
	int				width;
	int				height;
	int				timers[8];
	float			deltatime;
	t_gameobject	*player;
	t_gameobject	*exit;
	t_map			*mapa;
	t_bitmap		*screen;
	t_font			font;
	int				keys[512];
	int				last_key;
	t_rect			bound;
	int				moves_count;
	int				end_game;
	uint64_t		time_elapse;
	uint64_t		delay_elapse;
	uint64_t		time_offset;
	int				count_fps;
	int				scree_fps;
	int				tick_per_frame;
	float			fps_avg;
	int				time;
	int				last_time;
	int				off_set;
	int				colected;
	int				shake;
	int				shake_timer;
	t_point			p;
	int				game_state;
	SpriteBatch     *render;
	PrimitivesBatch *lines;
	Assets          *assets;
	PostProcessor   *post;
	Camera2D camera;
}t_game;

t_game				*create_game(char *mapa, int off_set);
void				destroy_game(t_game *g);
void				load_images(t_game *g);
void				game_load(t_game *g);
void				shake_screen(t_game *game);
void				game_mainloop(t_game *game);
void				game_print_data(t_game *game);
int					create_libx(t_game *game);
char				**read_map(char *path);
void				free_map(char **map);
int					check_map(t_map *map);
void				glib_add(t_game *game, char *filename, int id);
t_glib				*init_glib(void);
void				free_glib(t_game *game, t_glib *lib);
int					glib_contain_graph(t_glib *lib, int id);
t_graph				*glib_get_graph(t_game *lib, int id);
unsigned int		mlx_get_pixel(t_bitmap *img, int x, int y);
void				mlx_draw_pixel(t_game *img, int x, int y, int color);
unsigned int		mlx_get_color(int a, int r, int g, int b);
void				free_display(void *xvar);
t_bitmap			*mlx_load_bitmap(void *mlx, char *filename);
t_bitmap			*mlx_create_bitmap(void *mlx, int width, int height);
void				init_font(t_game *g);
void				print_text(t_game *g, const char *text, int x, int y);
int					text_size(t_game *g, const char *text);
void				mlx_destroy_bitmap(t_game *game, t_bitmap *bitmap);
void				draw_rect(t_game *g, t_rect r, int color);
void	            draw_solid_rect(t_game *g, t_rect r, int hexValue);
void				draw_tiles_onscreen(t_game *game);
void				put_pixel(t_bitmap *g, int x, int y, int color);
void				clear_bitmap(t_bitmap *g, int color);
void				draw_sprite(t_game *game, int id, int x, int y);
void				draw_alpha_sprite(t_game *g, int id, int x, int y);
void				draw_alpha_scale_sprite(t_game *g,int w,int h, int id, int x, int y);
t_gameobject		*create_player(t_game *g, int x, int y, int graph);
void				player_update_ex(t_gameobject *player, t_game *game);
t_gameobject		*create_player_bullet(t_game *g, int x, int y, int dir);
t_gameobject		*create_enemy_bullet(t_game *g, int x, int y, int dir);
void				enemy_shot_player(t_game *game, t_gameobject *b);
int					angle_offset(int index);
t_gameobject		*create_explosion(t_game *g, int x, int y);
t_gameobject		*create_enemy(t_game *g, int x, int y);
void				enemy_update(void *g, t_gameobject *b);
t_gameobject		*create_gameobject(t_game *g, int x, int y, int graph);
t_gameobject		*create_gameobject_last(t_game *g, int x, int y, int graph);
t_gameobject		*create_collect(t_game *g, int x, int y, int graph);
t_gameengine		*game_objects_create(void);
void				game_objects_destroy(t_gameengine *lib);
void				game_objects_update(t_game *g);
void				game_objects_render(t_game *g);
void	            game_objects_draw(t_game *g);
int					game_objects_add(t_gameengine *lib, t_gameobject *obj);
int					game_objects_add_back(t_gameengine *lib, t_gameobject *obj);
t_gameobject		*game_ojects_get(t_game *g, int type);
void				game_objects_clean(t_game *g);
int					game_objects_out_screen(t_game *g, t_gameobject *o);
t_parengine			*game_emitter_create(void);
t_emitter			*create_smoke_emitter(t_game *g, int x, int y, int loop);
void				game_emitter_destroy(t_parengine *lib);
void				game_emitter_update(t_game *g);
void				game_emitter_clean(t_game *g);
void				particle_init(t_particle *particle, int x, int y);
int					game_emitter_add(t_parengine *lib, t_emitter *obj);
t_emitter			*create_emitter(t_game *g, int x, int y);
int					place_free(t_game *g, t_gameobject *a, int x, int y);
int					place_empty(t_game *g, t_gameobject *a, int x, int y);
t_gameobject		*place_meeting(t_game *g, t_gameobject *a, int x, int y);
t_gameobject		*colide_type(t_game *g, t_gameobject *a, int type);
int					collide_with(t_gameobject *a, t_gameobject *e, \
								int x, int y);
t_rect				c_rect(int x, int y, int w, int h);
t_color				c_color(int r, int g, int b, int a);
t_point				c_point(int x, int y);
const char			*ft_format_text(const char *text, ...);
int					ft_isfile_extension(const char *file_name, const char *ext);
const char			*ft_file_extension(const char *file_name);
int					ft_file_exists(char *str);
int	                ft_strpos(const char *s, int c);
int					irandom(int min, int max);
float				frandom(float min, float max);
uint64_t			get_timer_value(void);
int					get_time(t_game *g);
int					get_time_now(void);
void				ft_sleep(float ms);
void				init_timer(t_game *g);
void				updade_timer(t_game *g);
int					ft_clamp(int value, int min, int max);
int					ft_cap(int value, int min, int max);
int					ft_mod(int a, int b);
float				ft_angle(t_gameobject *a, t_gameobject *b);
double				ft_distance(double x1, double y1, double x2, double y2);
int					line_in_rect(t_rect l, t_rect r);
int					rect_contains_point(t_rect r, t_point p);
int					linrect(t_point p1, t_point p2, t_rect r);
double				ease_in_out_circle(double t, double b, double c, double d);
double				ease_out_sin(double t, double b, double c, double d);
double				ease_in_sin(double t, double b, double c, double d);
double				ease_in_out_quad(double time, double b, double c, double d);
double				ease_in_back(double t, double b, double c, double d);

int	run_game(void);
#endif
