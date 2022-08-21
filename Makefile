
NAME			=	so_long


SRC = glad.c so_long.c Engine.c   map_load.c  map_check.c \
 libx_utils.c game_player.c game_player_bullet.c game_particles.c \
 game_objects.c game_objects_extra.c \
game_objects_add.c game_math.c game_load.c game_gems.c game_font.c \
game_explosion.c game_enemy.c game_enemy_bullet.c game_emitter.c \
game_ease.c game_core.c game_colider.c func_extra.c draw_utils.c game_timer.c

OBJS			=	${addprefix src/,${SRC:.c=.o}}




HEAD			=	-Iinclude -I./include

CC				=	clang

CFLAGS			=	-DPLATFORM_LINUX -I include -Wall  -g #-fsanitize=address
MLX_FLAGS       = -lSDL2 -lGL -ldl -lm

.c.o			:
					${CC} ${CFLAGS} ${HEAD} -c $< -o ${<:.c=.o}

$(NAME)			:	${OBJS}

					${CC} ${CFLAGS}  ${OBJS} -o ${NAME}  ${MLX_FLAGS}

all				:	${NAME}

val				:	${NAME}
					valgrind \
					--leak-check=full --tool=memcheck \
					--show-reachable=yes \
					--track-fds=yes \
					--errors-for-leak-kinds=all \
					--show-leak-kinds=all ./${NAME}

clean			:
				@rm -rf ${OBJS} 

fclean			:	clean
					@rm -rf ${NAME}

re				:	fclean all

.PHONY			:	all clean fclean re

clean:
				$(RM) $(OBJS) 

fclean:		clean
		$(RM) $(NAME) 

re:				
	fclean $(NAME)

.PHONY:			all clean fclean re
