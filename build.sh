gcc -Wall -Wextra -g -Iinclude -Igame -c /media/djoker/data/code/c/solonggl/src/Engine.c -o obj/Debug/Engine.o
gcc -Wall -Wextra -g -Iinclude -Igame -c /media/djoker/data/code/c/solonggl/src/game/draw_utils.c -o obj/Debug/game/draw_utils.o
gcc -Wall -Wextra -g -Iinclude -Igame -c /media/djoker/data/code/c/solonggl/src/game/func_extra.c -o obj/Debug/game/func_extra.o
gcc -Wall -Wextra -g -Iinclude -Igame -c /media/djoker/data/code/c/solonggl/src/game/game_colider.c -o obj/Debug/game/game_colider.o
gcc -Wall -Wextra -g -Iinclude -Igame -c /media/djoker/data/code/c/solonggl/src/game/game_core.c -o obj/Debug/game/game_core.o
gcc -Wall -Wextra -g -Iinclude -Igame -c /media/djoker/data/code/c/solonggl/src/game/game_ease.c -o obj/Debug/game/game_ease.o
gcc -Wall -Wextra -g -Iinclude -Igame -c /media/djoker/data/code/c/solonggl/src/game/game_emitter.c -o obj/Debug/game/game_emitter.o
gcc -Wall -Wextra -g -Iinclude -Igame -c /media/djoker/data/code/c/solonggl/src/game/game_enemy.c -o obj/Debug/game/game_enemy.o
gcc -Wall -Wextra -g -Iinclude -Igame -c /media/djoker/data/code/c/solonggl/src/game/game_enemy_bullet.c -o obj/Debug/game/game_enemy_bullet.o
gcc -Wall -Wextra -g -Iinclude -Igame -c /media/djoker/data/code/c/solonggl/src/game/game_explosion.c -o obj/Debug/game/game_explosion.o
gcc -Wall -Wextra -g -Iinclude -Igame -c /media/djoker/data/code/c/solonggl/src/game/game_font.c -o obj/Debug/game/game_font.o
gcc -Wall -Wextra -g -Iinclude -Igame -c /media/djoker/data/code/c/solonggl/src/game/game_gems.c -o obj/Debug/game/game_gems.o

gcc -Wall -Wextra -g -Iinclude -Igame -c /media/djoker/data/code/c/solonggl/src/minilibx.c -o obj/Debug/minilibx.o
gcc  -o bin/Debug/minilibx obj/Debug/Engine.o obj/Debug/game/draw_utils.o obj/Debug/game/func_extra.o obj/Debug/game/game_colider.o obj/Debug/game/game_core.o obj/Debug/game/game_ease.o obj/Debug/game/game_emitter.o obj/Debug/game/game_enemy.o obj/Debug/game/game_enemy_bullet.o obj/Debug/game/game_explosion.o obj/Debug/game/game_font.o obj/Debug/game/game_gems.o obj/Debug/game/game_load.o obj/Debug/game/game_math.o obj/Debug/game/game_objects.o obj/Debug/game/game_objects_add.o obj/Debug/game/game_objects_extra.o obj/Debug/game/game_particles.o obj/Debug/game/game_player.o obj/Debug/game/game_player_bullet.o obj/Debug/game/game_timer.o obj/Debug/game/image_list.o obj/Debug/game/libx_utils.o obj/Debug/game/map_check.o obj/Debug/game/map_load.o obj/Debug/glad.o obj/Debug/main.o obj/Debug/minilibx.o  -lSDL2 -lm -ldl -lGL  

/home/djoker/code/emsdk/upstream/emscripten/em++   -o    /home/djoker/code/crosside/projects/Web//glfw.html    /home/djoker/code/crosside/projects/obj/Web/glfw/glfw.o    -L/home/djoker/code/crosside/modules/sdl2/Web    -L/home/djoker/code/crosside/modules/json/Web    -L/home/djoker/code/crosside/modules/sdl2/Web    -s    USE_SDL=2    -s    FULL_ES3=1    -s    MIN_WEBGL_VERSION=2    -s    TOTAL_MEMORY=67108864    -s    FORCE_FILESYSTEM=1    -D_DEFAULT_SOURCE    -fPIC    -L/home/djoker/code/crosside/modules/raylib/Web    -L/home/djoker/code/crosside/modules/json/Web    -L/home/djoker/code/crosside/modules/sdl2/Web    -L/home/djoker/code/crosside/modules/raylib/Web    -lraylib    -s    USE_GLFW=3    -D_DEFAULT_SOURCE    -fPIC    --shell-file    /home/djoker/code/crosside/modules/raylib/shell.html    -L/home/djoker/code/crosside/libs/Web    -s    ASYNCIFY  
   
   
emcc  -o    /media/djoker/data/code/c/solonggl/src/build/web/so_long.html    
/home/djoker/code/crosside/projects/sdl/obj/Web/opengl_2/opengl_2.o    -L/home/djoker/code/crosside/modules/glfw/Web    -L/home/djoker/code/crosside/modules/glfw/Web    -L/home/djoker/code/crosside/modules/glfw/Web    -lglfw    -s    USE_GLFW=3    -s    MIN_WEBGL_VERSION=2    --preload-file    /home/djoker/code/crosside/projects/sdl/media@media    -L/home/djoker/code/crosside/modules/sdl2/Web    -L/home/djoker/code/crosside/modules/glfw/Web    -L/home/djoker/code/crosside/modules/glfw/Web    -L/home/djoker/code/crosside/modules/json/Web    -L/home/djoker/code/crosside/modules/sdl2/Web    -s    USE_SDL=2    -s    FULL_ES3=1    -s    MIN_WEBGL_VERSION=2    -s    TOTAL_MEMORY=67108864    -s    FORCE_FILESYSTEM=1    -D_DEFAULT_SOURCE    -fPIC    --preload-file    /home/djoker/code/crosside/projects/sdl/media@media    -L/home/djoker/code/crosside/libs/Web    -s    ASYNCIFY  
Done :) 
