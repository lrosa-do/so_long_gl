/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_check.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lrosa-do <lrosa-do@student.42lisboa>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/01 18:50:56 by igvaz-fe          #+#    #+#             */
/*   Updated: 2022/05/01 07:56:28 by lrosa-do         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

static int	is_retangular(char **map)
{
	int	i;

	i = 1;
	if (!map)
		return (0);
	while (map[i] != NULL)
	{
		if (strlen(map[i]) != strlen(map[0]))
			return (0);
		i++;
	}
	return (1);
}

static int	has_walls(char **map)
{
	int	i;
	int	j;
	int	len;

	j = 0;
	i = 0;
	while (map[i] != NULL)
		i++;
	while (map[0][j] != '\0' && map[i - 1][j] != '\0')
	{
		if (map[0][j] != '1' || map[i - 1][j] != '1')
			return (0);
		j++;
	}
	i = 1;
	len = strlen(map[i]);
	while (map[i] != NULL)
	{
		if (map[i][0] != '1' || map[i][len - 1] != '1')
			return (0);
		i++;
	}
	return (1);
}

static int	has_positions(t_map *map)
{
	int	i;
	int	j;

	i = 0;
	map->width = strlen(map->map[0]) * TILE_WIDTH;
	while (map->map[i] != NULL)
	{
		j = 0;
		while (map->map[i][j] != '\0')
		{
			if (map->map[i][j] == 'P')
				map->player++;
			else if (map->map[i][j] == 'E')
				map->exit++;
			else if (map->map[i][j] == 'C')
				map->colect++;
			j++;
		}
		i++;
	}
	map->height = i * TILE_HEIGHT;
	if (map->player != 1 || map->exit == 0 || map->colect == 0)
		return (0);
	return (1);
}

static int	is_validate(char **map)
{
	int	i;
	int	j;

	i = 0;
	while (map[i] != NULL)
	{
		j = 0;
		while (map[i][j] != '\0')
		{
			if (map[i][j] != 'P' && map[i][j] != 'E' && map[i][j] != 'C'
				&& map[i][j] != '0' && map[i][j] != '1' && map[i][j] != '2')
				return (0);
			j++;
		}
		i++;
	}
	return (1);
}

int	check_map(t_map *map)
{
	SDL_Log("Is rectangular");
	if (!is_retangular(map->map))
	{
		SDL_Log("Error\nMap is not rectangular.\n");
		return (0);
	}
	SDL_Log("As walls ");
	
	if (!has_walls(map->map))
	{
		SDL_Log("Error\nMap is not surrounded by walls. \n");
		return (0);
	}
	SDL_Log("as positions");
	
	if (!has_positions(map))
	{
		SDL_Log("Error\nMap does not have  Objects. \n");
		return (0);
	}
	SDL_Log("Is alid");
	
	if (!is_validate(map->map))
	{
		SDL_Log("Error\nMap is not ok. \n");
		return (0);
	}
	return (1);
}
