#include <stdio.h>
#include <SDL2/SDL.h>
//#define GL_GLEXT_PROTOTYPES 1
//#include <SDL2/SDL_opengl.h>
//#include <SDL2/SDL_opengl_glext.h>
#include "glad.h"


#include <stdlib.h>
#include	"Engine.h"
#include	"so_long.h"

void	setup_game(t_game *game)
{
	game->end_game = 0;
	game->width = game->mapa->width;
	game->colected = 0;
	game->p.x = 0;
	game->p.y = 0;
	game->shake = 0;
	game->height = game->mapa->height + game->off_set;
}

t_game	*create_game(char *mapa, int off_set)
{
	t_game	*game;



	game = (t_game *)mem_malloc(sizeof(t_game));
	if (!game)
		return (NULL);
	game->mapa = (t_map *)mem_malloc(sizeof(t_map));
	if (!game->mapa)
		return (NULL);
	game->mapa->colect = 0;
	game->mapa->player = 0;
	game->mapa->exit = 0;
	game->width=640;
	game->height=544;
	game->mapa->width=640;
	game->mapa->height=544;

    SDL_Log("read");

	game->mapa->map=(char**) mem_malloc(sizeof(char*) * 17);
	for (int i=0;i<16;i++)
	{
	//	game->mapa->map[i]=(char*) mem_malloc(sizeof(char) * 20);
	}

game->mapa->map[0 ] = strdup("11111111111111111111");
game->mapa->map[1 ] = strdup("1CCC0000000C00000001");
game->mapa->map[2 ] = strdup("1CCC0000000C00000001");
game->mapa->map[3 ] = strdup("1CC2C00000CCC0000001");
game->mapa->map[4 ] = strdup("100C0000000C00000001");
game->mapa->map[5 ] = strdup("10000CCC00000200CC01");
game->mapa->map[6 ] = strdup("100000000000C0000001");
game->mapa->map[7 ] = strdup("10000000000CCC000001");
game->mapa->map[8 ] = strdup("1000CC000020C0000001");
game->mapa->map[9 ] = strdup("1000CC00000000000001");
game->mapa->map[10 ] =strdup("1000CC00E00000000001");
game->mapa->map[11 ] = strdup("10000000000C00000001");
game->mapa->map[12 ] = strdup("10000000000C00000001");
game->mapa->map[13 ] = strdup("100C0000000000C0C001");
game->mapa->map[14 ] = strdup("10C2C000000000000001");
game->mapa->map[15 ] = strdup("100C0000000000000P01");
game->mapa->map[16 ] = strdup("11111111111111111111");

	
//	game->mapa->map = read_map(mapa);
	SDL_Log("cheak");
	if (!check_map(game->mapa))
	{
		SDL_Log("fail");
		destroy_game(game);
		return (NULL);
	}
	
	SDL_Log("setup %d %d ",game->mapa->width,game->mapa->height);
	game->off_set = off_set;
	setup_game(game);

SDL_Log("create sdl");
	if (!create_libx(game))
	{
		SDL_Log("fail");
		destroy_game(game);
		return (NULL);
	}

	return (game);
}



int	main(void)
{
	SDL_Log("INIT");
	SDL_Init( SDL_INIT_VIDEO   );

	t_game	*game;
	printf("INIT");

	game = create_game("assets/maps/map3.ber", 22);
	if (!game)
	{
		destroy_game(game);
		return (1);
	}

	return (0);
}
/*

int	main(int argc, char **args)
{
	t_game	*game;

	if (argc != 2)
	{
		ft_printf("Error\n Please type the map to be load. \n");
		return (1);
	}
	if (ft_file_exists(args[1]) && (ft_isfile_extension(args[1], ".ber")))
	{
		game = create_game(args[1], 22);
		if (!game)
			return (1);
		destroy_game(game);
	}
	else
		ft_printf("Error\n File %s dont exists or is not valid .\n", args[1]);
	return (0);
}
*/


