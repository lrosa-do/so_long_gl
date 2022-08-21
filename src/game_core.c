/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_core.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lrosa-do <lrosa-do@student.42lisboa>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/10 20:51:26 by lrosa-do          #+#    #+#             */
/*   Updated: 2022/06/05 07:24:49 by lrosa-do         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"
#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>
#endif



t_game *main_game;
SDL_Window* gWindow = NULL;
SDL_GLContext gContext =NULL;



void	game_on_game(t_game *game)
{


SpriteBatch *render =game->render;

/*
BeginSpriteBatch(game->render,game->camera);
BeginPrimitivesBatch(game->lines);
draw_tiles_onscreen(game);
game_objects_update(game);
game_emitter_update(game);
EndSpriteBatch(render);
EndPrimitivesBatch(game->lines,game->camera);
BeginSpriteBatch(game->render,game->camera);
game_print_data(game);
EndSpriteBatch(render);

*/
    //game->post->Blur=1;
	//game->post->Chaos=1;
    BeginPostProcessor(game->post);
    BeginSpriteBatch(game->render,game->camera);
    BeginPrimitivesBatch(game->lines);
	draw_tiles_onscreen(game);
	game_objects_update(game);
	game_emitter_update(game);
	EndSpriteBatch(render);
	EndPrimitivesBatch(game->lines,game->camera);
	EndPostProcessor(game->post);
	RenderPostProcessor(game->post,game->deltatime/2);
	BeginSpriteBatch(game->render,game->camera);
	game_print_data(game);
	EndSpriteBatch(render);


}

void	game_on_game_over(t_game *game)
{
	int	w;
	int	h;

	w = (game->width / 2) - (8 * 12);
	h = (game->height / 2);


//Assets *assets = game->assets;
SpriteBatch *render =game->render;



    BeginSpriteBatch(game->render,game->camera);


	print_text(game, "GAME OVER", w, h - 20);
	print_text(game, "YOU LOSE", w + 12, h + 20);
	print_text(game, "PRESS ESQ KEY TO EXIT", w, game->height - 26);
	if (game->last_key == KEY_ESC)
		game->end_game = 1;


	EndSpriteBatch(render);

}

void	game_on_game_win(t_game *game)
{
	int	w;
	int	h;
//Assets *assets = game->assets;
SpriteBatch *render =game->render;



    BeginSpriteBatch(game->render,game->camera);


	w = (game->width / 2) - (8 * 12);
	h = (game->height / 2);

	print_text(game, "GAME OVER", w, h - 20);
	print_text(game, "YOU WIN", w + 12, h + 20);
	print_text(game, "PRESS ESQ KEY TO EXIT", w, game->height - 26);
	if (game->last_key == KEY_ESC)
		game->end_game = 1;


	EndSpriteBatch(render);

}

void	game_mainloop(t_game *game)
{
	if (game->game_state == 0)
		game_on_game(game);
	if (game->game_state == 1)
		game_on_game_over(game);
	if (game->game_state == 2)
		game_on_game_win(game);
}


void	shake_screen(t_game *game)
{
	int	x;

	x = 0;
	if (game->shake)
	{
		if (get_time(game) > game->shake_timer + 2)
		{
			game->shake = 0;
			game->post->Shake = 0;
			game->p = c_point(0, 0);
		}
		/*while (x < 20)
		{
			game->p.x = (int)(frandom(-2, 2));
			game->p.y = (int)(frandom(-2, 2));
			x+=2;
		}*/

	}
}

void	destroy_game(t_game *g)
{
	if (!g)
		return ;
	SDL_Log("FRee map")	;
	//free(g->mapa->map);
	free_map(g->mapa->map);
	mem_free(g->mapa);
	SDL_Log("FRee game")	;
	mem_free(g);
}

int	on_close(t_game *game)
{

	SDL_Log("Clean memory");
    game_objects_destroy(game->gameobjects);
	game_emitter_destroy(game->gamepar);
	FreeAssetManager(game->assets);
	FreePrimitivesBatch(game->lines);
	FreePostProcessor(game->post);
	FreeSpriteBatch(game->render);
	FreeGraphic(game->font.image);
	FreeTexture(game->font.texture);



	destroy_game(game);

	SDL_GL_DeleteContext(gContext);
    SDL_DestroyWindow( gWindow );
    gWindow = NULL;
	mem_dump(0);
    SDL_Log("BY! BY! :) \n");
    SDL_Quit();
	
	exit(0);
	return (0);
}

int	on_key_down(int keycode, t_game *game)
{
	if (!game )
		return (1);

   // SDL_Log("%d",keycode);
    if (keycode>=255)
	{
        keycode=255;
	}

	if (keycode == 27)
	{
		on_close(game);
	}
	game->keys[keycode] = 1;
	game->last_key = keycode;
	return (0);
}


int	on_key_up(int keycode, t_game *game)
{
	if (!game )
		return (1);

   // SDL_Log("up %d",keycode);
    if (keycode >= 255)
	{
        keycode=255;
	}

	game->keys[keycode] = 0;
	game->last_key = keycode;
	return (0);
}
int	on_render(t_game *game)
{
		if (!game || game->end_game)
		on_close(game);


	ClearCamera(game->camera);




    game->camera.position = (Vec2){ 0, 0 };
    game->camera.offset = (Vec2){ 0,0};
    game->camera.rotation = 0.0f;
    game->camera.zoom = 1.0f;
    game->camera.bound = (Rect){0,0,game->width,game->height};
	game->camera.color=BLACK;
    UpdateCamera(&game->camera);


	updade_timer(game);
	shake_screen(game);
	game_mainloop(game);

	for (int i=0;i<255;i++)
		game->keys[i] = 0;


	game->last_key = -1;
	return (0);
}



int	sdl_loop ()
{
   
    int doClose = 0; 
    SDL_Event   events;
			while( SDL_PollEvent( &events ) != 0 )
			{


				switch (events.type)
				{
                        case SDL_QUIT:
                        {
                            //doClose = 1;
							//on_close(main_game);
                            break;

                        }
                         case SDL_WINDOWEVENT:
                        {


                              if (events.window.event == SDL_WINDOWEVENT_RESIZED)
                              {
									break;
                            }

                              else  if (events.window.event ==  SDL_WINDOWEVENT_CLOSE)
                              {
                                             //doClose = 1;
							                 on_close(main_game);
                            break;

                              }
                        break;
                        }

                   

                       case SDL_KEYDOWN:
                       {
                           // key.keysym.scancode,key.keysym.sym,key.keysym.mod);
                          // printf("scan:%d  key:%d, \n",events.key.keysym.scancode,events.key.keysym.sym);

                          // add_key_down(on_key_down);
						      on_key_down(events.key.keysym.sym,main_game);

                       }
                       break;
                       case SDL_KEYUP:
                       {

								on_key_up(events.key.keysym.sym,main_game);

                            //add_key_up(events.key.keysym.sym);

                       }
                       break;

                        case SDL_MOUSEBUTTONDOWN:
                        {


                                  //  core->events[e].hook(events.button.x,events.button.y,events.button.button -1, core->events[e].param);
						break;

                        }
                        break;
                        case SDL_MOUSEBUTTONUP:
                        {
                                   
                           
                        }
                        break;
                        case SDL_MOUSEMOTION:
                        {
                            
                                //    core->events[e].hook(events.button.x,events.button.y,events.button.button -1, core->events[e].param);
                                   
                        }
                        break;
                    }
                break;
			}
     

     
    return (0);
}
void game_loop()
{

    sdl_loop();
    on_render(main_game);     
	SDL_GL_SwapWindow( gWindow );
    
         
}
int	create_libx(t_game *game)
{

	



  // Uint32 flags =   SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL;
  //flags =   SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN |SDL_WINDOW_OPENGL;


    // gWindow = SDL_CreateWindow(     TITLE,     SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,     SCREEN_WIDTH,SCREEN_HEIGHT,  flags );

gWindow= SDL_CreateWindow("djokersoft", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
 game->width, game->height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);


SDL_Log("CONFIG GL");
/*
SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0); // or 1*/


SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    SDL_GL_SetSwapInterval(1);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);



SDL_Log("create context");

    gContext = SDL_GL_CreateContext( gWindow );

SDL_Log("load glad");

	gladLoadGLES2Loader(SDL_GL_GetProcAddress);
SDL_Log("create assets manager");
    game->assets= CreateAssetsManager();

   game->post = CreatePostProcessor(game->width,game->height);
//	SDL_assert(game->post !=NULL);

SDL_Log("create sprite batch");

	game->render= CreateSpriteBatch(5000);
SDL_Log("create lines batch");
    game->lines= CreatePrimitivesBatch(5000);

SDL_Log("create post process");


 //   LoadAtlasText(game->assets,"assets/atlas.txt","assets/Atlas.png");
SDL_Log("Load assets");

#if defined(PLATFORM_LINUX)
chdir("/media/djoker/data/code/c/solonggl/build");    
#endif

SDL_Log("load images");

 Assets *assets = game->assets;
 LoadGraphicAsset(assets, "assets/images/tile.xpm", 0);
 LoadGraphicAsset(assets, "assets/images/rock.xpm", 1);
 LoadGraphicAsset(assets, "assets/images/rock2.xpm", 2);
 LoadGraphicAsset(assets, "assets/images/exit.xpm", 20);
 LoadGraphicAsset(assets, "assets/images/start.xpm", 21);
 LoadGraphicAsset(assets, "assets/images/gem/gem_0.xpm", 200);
 LoadGraphicAsset(assets, "assets/images/gem/gem_1.xpm", 201);
 LoadGraphicAsset(assets, "assets/images/gem/gem_2.xpm", 202);
 LoadGraphicAsset(assets, "assets/images/gem/gem_3.xpm", 203);
 LoadGraphicAsset(assets, "assets/images/gem/gem_4.xpm", 204);
 LoadGraphicAsset(assets, "assets/images/bullet.xpm", GAME_PLAYER_BULLET);
 LoadGraphicAsset(assets, "assets/images/bullet-enemy.xpm", GAME_ENEMY_BULLET);
 LoadGraphicAsset(assets, "assets/images/tank/tank_end.xpm", GAME_ENEMY - 1);
 LoadGraphicAsset(assets, "assets/images/tank/tank.xpm", GAME_ENEMY);
 LoadGraphicAsset(assets, "assets/images/tank/cano_0.xpm", 300);
 LoadGraphicAsset(assets, "assets/images/tank/cano_1.xpm", 301);
 LoadGraphicAsset(assets, "assets/images/tank/cano_2.xpm", 302);
 LoadGraphicAsset(assets, "assets/images/tank/cano_3.xpm", 303);
 LoadGraphicAsset(assets, "assets/images/tank/cano_4.xpm", 304);
 LoadGraphicAsset(assets, "assets/images/tank/cano_5.xpm", 305);
 LoadGraphicAsset(assets, "assets/images/tank/cano_6.xpm", 306);
 LoadGraphicAsset(assets, "assets/images/tank/cano_7.xpm", 307);
 LoadGraphicAsset(assets, "assets/images/player/player_0.xpm", 100);
 LoadGraphicAsset(assets, "assets/images/player/player_1.xpm", 101);
 LoadGraphicAsset(assets, "assets/images/player/player_2.xpm", 102);
 LoadGraphicAsset(assets, "assets/images/player/player_3.xpm", 103);
 LoadGraphicAsset(assets, "assets/images/player/player_4.xpm", 104);
 LoadGraphicAsset(assets, "assets/images/player/player_5.xpm", 105);
 LoadGraphicAsset(assets, "assets/images/player/player_6.xpm", 106);
 LoadGraphicAsset(assets, "assets/images/player/player_7.xpm", 107);
 LoadGraphicAsset(assets, "assets/images/explode/explode_0.xpm", GAME_EXPLOSION);
 LoadGraphicAsset(assets, "assets/images/explode/explode_1.xpm", 451);
 LoadGraphicAsset(assets, "assets/images/explode/explode_2.xpm", 452);
 LoadGraphicAsset(assets, "assets/images/explode/explode_3.xpm", 453);
 LoadGraphicAsset(assets, "assets/images/explode/explode_4.xpm", 454);
 LoadGraphicAsset(assets, "assets/images/explode/explode_5.xpm", 455);
 LoadGraphicAsset(assets, "assets/images/explode/explode_6.xpm", 456);
 LoadGraphicAsset(assets, "assets/images/explode/explode_7.xpm", 457);
 LoadGraphicAsset(assets, "assets/images/explode/explode_8.xpm", 458);
 LoadGraphicAsset(assets, "assets/images/explode/explode_9.xpm", 459);
 LoadGraphicAsset(assets, "assets/images/explode/explode_10.xpm", 460);
 LoadGraphicAsset(assets, "assets/images/explode/explode_11.xpm", 461);
 LoadGraphicAsset(assets, "assets/images/explode/explode_12.xpm", 462);
 LoadGraphicAsset(assets, "assets/images/explode/explode_13.xpm", 463);
 SDL_Log("generetae atlash");

 GererateGraphicsAtlas(assets,1,1,0);

	game->bound = c_rect(0, 0, game->width, game->height);

	SDL_Log("create objects list");

	game->gameobjects = game_objects_create();
	game->gamepar = game_emitter_create();

SDL_Log("load game");
	game_load(game);
	
	main_game = game;

	#if defined(PLATFORM_WEB)
        emscripten_set_main_loop(game_loop, 0, 1);
    #else

    while (!sdl_loop())
	{

           on_render(game);     

            SDL_GL_SwapWindow( gWindow );
	}
    #endif

	
	//mlx_loop_hook(game->mlx, &on_render, game);

	return (1);
}
