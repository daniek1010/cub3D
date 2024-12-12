/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ojacobs <ojacobs@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 00:42:14 by ojacobs           #+#    #+#             */
/*   Updated: 2024/12/12 19:30:10 by ojacobs          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef GAME_H
# define GAME_H

# define WIDTH 1280
# define HEIGHT 720


# define BLOCK 64
# define DEBUG 1

# define W 119
# define A 97
# define S 115
# define D 100
# define LEFT 65361
# define RIGHT 65363
# define KEY_ESC 65307
# define PI 3.14159265359

# include "../mlx/mlx.h"
# include "./parsing.h"

#include <fcntl.h> 
#include <float.h>
#include <unistd.h>
#include <stdio.h> 
#include <stdlib.h>
# include <stdbool.h>
# include <math.h>

typedef struct s_parser	t_parser;
typedef struct s_texture	t_texture;
typedef struct s_map	t_map;
// typedef struct s_texture t_texture;
// typedef struct s_ray t_ray;

// typedef struct s_ray
// {
// 	double	camera_x;
// 	double	dir_x;
// 	double	dir_y;
// 	int		map_x;
// 	int		map_y;
// 	int		step_x;
// 	int		step_y;
// 	double	sidedist_x;
// 	double	sidedist_y;
// 	double	deltadist_x;
// 	double	deltadist_y;
// 	double	wall_dist;
// 	double	wall_x;
// 	int		side;
// 	int		line_height;
// 	int		draw_start;
// 	int		draw_end;
// }	t_ray;


typedef struct s_player
{
	float	x;
	float	y;
	float	player_x;
	float	player_y;
	float	angle;
	
	//need to init
	int		speed;
	float	cos_angle;
	float	sin_angle;

	
	bool	key_up;
	bool	key_down;
	bool	key_left;
	bool	key_right;
	bool	left_rotate;
	bool	right_rotate;
}	t_player;


// typedef struct s_game
// {
//     float ray_dir_x;
//     float ray_dir_y;
//     float delta_dist_x;
//     float delta_dist_y;
//     float side_dist_x;
//     float side_dist_y;
//     int step_x;
//     int step_y;
//     int side; // 0 for vertical wall, 1 for horizontal wall
//     float perp_wall_dist;
//     int line_height;
//     int draw_start;
//     int draw_end;
//     int tex_x;
//     int tex_y;
//     int screen_x;
//     int color;
//     int is_ceiling;
//     t_parser *element;
//     char **map;
//     void *mlx;
//     void *win;
//     void *img;
// 	t_player	player;
// } t_game;

typedef struct s_game
{
	void		*mlx;
	void		*win;
	void		*img;
	char		*data;
	
	int			color;
	int			bpp;
	int			size_line;
	int		tmp_size_line;
	int			endian;
	
	t_player	player;
	t_parser	*element;
	char		**map;
	int			**tex_pixels;
	
	int			win_height;
	int			win_width;

	// Add raycasting-related variables
	float ray_dir_x;
    float ray_dir_y;
    float delta_dist_x;
    float delta_dist_y;
    float side_dist_x;
    float side_dist_y;
    int step_x;
    int step_y;
    int side;
    int line_height;
    int draw_start;
    int draw_end;
    int tex_x;
    int tex_y;
    int screen_x;
    float perp_wall_dist;
    int is_ceiling; 
}	t_game;

int draw_loops(t_game *game);
/*	ray_caster.c	*/
int		get_texture_pixel(t_texture *texture, int x, int y, void *texture_data);
void	init_ray(t_player *player, t_game *game, float ray_angle);
void	calculate_step_and_sidedist(t_player *player, t_game *game, t_map *map);
int		perform_dda(t_game *game, t_map *map);
float	calculate_wall_distance(t_player *player, t_game *game,
		t_map *map, int side);

/*	play_move.c	*/
// void	move_in_direction(t_player *player, t_game *game, int direction);
void	move_player(t_player *player, t_game *game);
void	rotate_player(t_player *player);
int		init_player(t_player *player, t_map *map);
int		set_player_angle(t_map *map, t_player *player);

/*	key_mapping.c	*/
int		key_press(int keycode, t_game *game);
int		key_release(int keycode, t_player *player);
int		close_game_on_cross(t_game *game);
int		close_game(t_game *game);
void	free_game_struct(t_game *game);

/*	mainc.c	*/
int		init_game(t_game *game, char *argv);

/*	ray_caster2.c	*/
void	draw_line(t_player *player, t_game *game, float ray_angle);
void	start_draw_line(t_game *game);
void	draw_ceiling_floor_wall(t_game *game);
void	draw_ceiling_floor(t_game *game);
void	draw_wall(t_game *game);

/*	mainnnc.c	*/
int		draw_loop(t_game *game);
int		load_textures(t_game *game);
void	clear_image(t_game *game);
void	draw_map(t_game *game);
void	draw_square(int x, int y, int size, t_game *game);
void	put_pixel(int x, int y, t_game *game);
bool	touch(float px, float py, t_game *game);


// void draw_line(t_player *player, t_game *game, float ray_angle, int screen_x);
 
#endif