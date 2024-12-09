/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_caster.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ojacobs <ojacobs@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 01:27:06 by ojacobs           #+#    #+#             */
/*   Updated: 2024/12/07 04:04:09 by ojacobs          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/game.h"

// int	get_texture_pixel(t_texture *texture, int x, int y, void *texture_data)
// {
// 	int				offset;
// 	unsigned char	*data;
// 	int				color;

// 	offset = (y * texture->size_line + x * (texture->bpp / 8));
// 	data = (unsigned char *)texture_data;
// 	color = (*(int *)(data + offset));
// 	return (color);
// }

// void	init_ray(t_player *player, t_game *game, float ray_angle)
// {
// 	game->ray_dir_x = cos(ray_angle);
// 	game->ray_dir_y = sin(ray_angle);
// 	game->delta_dist_x = fabs(1 / game->ray_dir_x);
// 	game->delta_dist_y = fabs(1 / game->ray_dir_y);
// }

// void	calculate_step_and_sidedist(t_player *player, t_game *game, t_map *map)
// {
// 	if (game->ray_dir_x < 0)
// 	{
// 		game->step_x = -1;
// 		game->side_dist_x = (player->player_x / BLOCK - map->x) \
// 		* game->delta_dist_x;
// 	}
// 	else
// 	{
// 		game->step_x = 1;
// 		game->side_dist_x = (map->x + 1.0 - player->player_x / BLOCK) \
// 		* game->delta_dist_x;
// 	}
// 	if (game->ray_dir_y < 0)
// 	{
// 		game->step_y = -1;
// 		game->side_dist_y = (player->player_y / BLOCK - map->y) \
// 		* game->delta_dist_y;
// 	}
// 	else
// 	{
// 		game->step_y = 1;
// 		game->side_dist_y = (map->y + 1.0 - player->player_y / BLOCK) \
// 		* game->delta_dist_y;
// 	}
// }

// int	perform_dda(t_game *game, t_map *map)
// {
// 	int	hit;

// 	hit = 0;
// 	while (!hit)
// 	{
// 		if (game->side_dist_x < game->side_dist_y)
// 		{
// 			game->side_dist_x += game->delta_dist_x;
// 			map->x += game->step_x;
// 			game->side = 0;
// 		}
// 		else
// 		{
// 			game->side_dist_y += game->delta_dist_y;
// 			map->y += game->step_y;
// 			game->side = 1;
// 		}
// 		if (game->map[map->y][map->x] == '1')
// 			hit = 1;
// 	}
// 	return (game->side);
// }

// float	calculate_wall_distance(t_player *player, t_game *game, \
// t_map *map, int side)
// {
// 	float	perp_wall_dist;

// 	if (side == 0)
// 		perp_wall_dist = (map->x - player->player_x / BLOCK + \
// 		(1 - game->step_x) / 2) / game->ray_dir_x;
// 	else
// 		perp_wall_dist = (map->y - player->player_y / BLOCK + \
// 		(1 - game->step_y) / 2) / game->ray_dir_y;
// 	return (perp_wall_dist);
// }