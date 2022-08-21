/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_load.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lrosa-do <lrosa-do@student.42lisboa>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/25 07:41:13 by lrosa-do          #+#    #+#             */
/*   Updated: 2022/05/01 11:32:10 by lrosa-do         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

#define LEN(arr) ((int) (sizeof (arr) / sizeof (arr)[0]))

void	*ft_memcpy(void *dst, const void *src, size_t len )
{
	size_t	i;

	i = 0;
	if (src == NULL && dst == NULL)
		return (NULL);
	while (i < len)
	{
		*((char *)dst + i) = *((char *)src + i);
		i++;
	}
	return (dst);
}

char	*ft_strdup(const char *str)
{
	size_t	len;
	char	*copy;

	len = strlen(str) + 1;
	copy = (char *) mem_malloc(sizeof(char) * len);
	if (!copy)
		return (NULL);
	ft_memcpy(copy, str, len);
	return (copy);
}

char	**read_map(char *path)
{
	
	char	**map;

char mapa[17][20]=
{
{"11111111111111111111\n"},
{"1CCC0000000C00000001\n"},
{"1CCC0000000C00000001\n"},
{"1CC2C00000CCC0000001"},
{"100C0000000C00000001"},
{"10000CCC00000200CC01"},
{"100000000000C0000001"},
{"10000000000CCC000001"},
{"1000CC000020C0000001"},
{"1000CC00000000000001"},
{"1000CC00E00000000001"},
{"10000000000C00000001"},
{"10000000000C00000001"},
{"100C0000000000C0C001"},
{"10C2C000000000000001"},
{"100C0000000000000P01"},
{"11111111111111111111"},
};
SDL_Log("process");


    SDL_Log("rows %d\n", LEN(mapa)); /*number of rows*/
    SDL_Log("columns %d\n", LEN(mapa[0])); /*number of columns*/
   // SDL_Log("%d\n", (int) strlen(mapa[1][1]) + 1); /*size of element*/

    int	x;
	int	y;

	int valuex;
	int valuey;

SDL_Log("copy");
map=(char**)malloc(20 * sizeof(char));
map[0 ] = strdup("11111111111111111111");
map[1 ] = strdup("1CCC0000000C00000001");
map[2 ] = strdup("1CCC0000000C00000001");
map[3 ] = strdup("1CC2C00000CCC0000001");
map[4 ] = strdup("100C0000000C00000001");
map[5 ] = strdup("10000CCC00000200CC01");
map[6 ] = strdup("100000000000C0000001");
map[7 ] = strdup("10000000000CCC000001");
map[8 ] = strdup("1000CC000020C0000001");
map[9 ] = strdup("1000CC00000000000001");
map[10 ] =strdup("1000CC00E00000000001");
map[11 ] = strdup("10000000000C00000001");
map[12 ] = strdup("10000000000C00000001");
map[13 ] = strdup("100C0000000000C0C001");
map[14 ] = strdup("10C2C000000000000001");
map[15 ] = strdup("100C0000000000000P01");
map[16 ] = strdup("11111111111111111111");
/*
	y = 0;
	while (y<=16)
	{
		//map[y] = strdup("11111111111111111111");

		x = 0;
		while (x<20)
		{
			valuex=mapa[y][x];
			map[y][x]=mapa[y][x];
			//SDL_Log("x:%c",mapa[y][x]);
			x++;
		}

		SDL_Log("%d %s %d",y,map[y],strlen(map[y]));
		y++;
	}
*/
//bzero(map,20 * sizeof(char));
//bzero(*map,20*sizeof(char));
 	
 //memcmp(*map,str,strlen(str));


	return (map);
}

void	free_map(char **map)
{
	int	i;

SDL_Log("free map ");

	i = 0;
	while (map[i] != 0)
	{
		free(map[i]);
		map[i] = NULL;
		i++;
	}
	mem_free(map);
	map = NULL;
}
