#include "../includes/game.h"

// our own put_pixel

void put_pixel(int x, int y, int color, t_game *game)
{
    if(x >= WIDTH || y >= HEIGHT || x < 0 || y < 0)
        return;
    
    int index = y * game->size_line + x * game->bpp / 8;
    game->data[index] = color & 0xFF;
    game->data[index + 1] = (color >> 8) & 0xFF;
    game->data[index + 2] = (color >> 16) & 0xFF;
}

// our own clear_image
void clear_image(t_game *game)
{
    for(int y = 0; y < HEIGHT; y++)
        for(int x = 0; x < WIDTH; x++)
            put_pixel(x, y, 0, game);
}

// utils functions
void draw_square(int x, int y, int size, int color, t_game *game)
{
    for(int i = 0; i < size; i++)
        put_pixel(x + i, y, color, game);
    for(int i = 0; i < size; i++)
        put_pixel(x, y + i, color, game);
    for(int i = 0; i < size; i++)
        put_pixel(x + size, y + i, color, game);
    for(int i = 0; i < size; i++)
        put_pixel(x + i, y + size, color, game);
}

void draw_map(t_game *game)
{
    char **map = game->map;
    int color = 0xFF0000;
    for(int y = 0; map[y]; y++)
        for(int x = 0; map[y][x]; x++)
            if(map[y][x] == '1')
                draw_square(x * BLOCK, y * BLOCK, BLOCK, color, game);
}

// distance calculation functions
float distance(float x, float y){
    return sqrt(x * x + y * y);
}

float fixed_dist(float x1, float y1, float x2, float y2, t_game *game)
{
    float delta_x = x2 - x1;
    float delta_y = y2 - y1;
    float angle = atan2(delta_y, delta_x) - game->player.angle;

    // Correct for fish-eye effect
    float raw_dist = distance(delta_x, delta_y);
    return raw_dist * cos(angle);
}


// float fixed_dist(float x1, float y1, float x2, float y2, t_game *game)
// {
//     float delta_x = x2 - x1;
//     float delta_y = y2 - y1;
//     float angle = atan2(delta_y, delta_x) - game->player.angle;
//     float fix_dist = distance(delta_x, delta_y) * cos(angle);
//     return fix_dist;
// }

// touch function 
bool touch(float px, float py, t_game *game)
{
    int x = px / BLOCK;
    int y = py / BLOCK;
    if(game->map[y][x] == '1' || game->map[y][x] == ' ')
        return true;
    return false;
}

void load_textures(t_game *game)
{
    // Load East texture
    game->element->texture->east_img = mlx_xpm_file_to_image(game->mlx, "./Tower_Blue.xpm", &game->element->texture->width, &game->element->texture->height);
    game->element->texture->east_data = mlx_get_data_addr(game->element->texture->east_img, &game->element->texture->bpp, &game->element->texture->size_line, &game->element->texture->endian);

    // Load West texture
    game->element->texture->west_img = mlx_xpm_file_to_image(game->mlx, "./Tower_Red.xpm", &game->element->texture->width, &game->element->texture->height);
    game->element->texture->west_data = mlx_get_data_addr(game->element->texture->west_img, &game->element->texture->bpp, &game->element->texture->size_line, &game->element->texture->endian);

    // Load North texture
    game->element->texture->north_img = mlx_xpm_file_to_image(game->mlx, "./Gate_Closed.xpm", &game->element->texture->width, &game->element->texture->height);
    game->element->texture->north_data = mlx_get_data_addr(game->element->texture->north_img, &game->element->texture->bpp, &game->element->texture->size_line, &game->element->texture->endian);

    // Load South texture
    game->element->texture->south_img = mlx_xpm_file_to_image(game->mlx, "./Mushroom.xpm", &game->element->texture->width, &game->element->texture->height);
    game->element->texture->south_data = mlx_get_data_addr(game->element->texture->south_img, &game->element->texture->bpp, &game->element->texture->size_line, &game->element->texture->endian);
}

// initialisation functions

void init_game(t_game *game, char *argv)
{
	game->element = parsing_func(argv);
    if (game->element == NULL)
        return;
    init_player(&game->player, game->element->map_array);
    game->tex_pixels = NULL;
    game->map = game->element->map_array->map;
    game->mlx = mlx_init();
    game->win = mlx_new_window(game->mlx, WIDTH, HEIGHT, "Game");
    game->img = mlx_new_image(game->mlx, WIDTH, HEIGHT);
    game->data = mlx_get_data_addr(game->img, &game->bpp, &game->size_line, &game->endian);
    load_textures(game);
    mlx_put_image_to_window(game->mlx, game->win, game->img, 0, 0);
}

int get_texture_pixel(void *texture_data, int x, int y, int size_line, int bpp)
{
    int offset = (y * size_line + x * (bpp / 8));  // Calculate the pixel offset
    unsigned char *data = (unsigned char *)texture_data;
    int color = (*(int *)(data + offset));  // Extract the pixel color
    return color;
}

// void draw_line(t_player *player, t_game *game, float ray_angle, int screen_x)
// {
//     float cos_angle = cos(ray_angle);
//     float sin_angle = sin(ray_angle);

//     float ray_x = player->playerX;
//     float ray_y = player->playerY;

//     int texture_direction = 0; // 0 = east, 1 = west, 2 = north, 3 = south
//     int hit_vertical = 0;      // 1 = vertical hit, 0 = horizontal hit

//     float delta_dist_x = fabs(1 / cos_angle);
//     float delta_dist_y = fabs(1 / sin_angle);

//     float step_x = (cos_angle > 0) ? 1 : -1;
//     float step_y = (sin_angle > 0) ? 1 : -1;

//     float side_dist_x = (cos_angle > 0) ? 
//         (floor(ray_x / BLOCK) * BLOCK + BLOCK - ray_x) * delta_dist_x :
//         (ray_x - floor(ray_x / BLOCK) * BLOCK) * delta_dist_x;

//     float side_dist_y = (sin_angle > 0) ? 
//         (floor(ray_y / BLOCK) * BLOCK + BLOCK - ray_y) * delta_dist_y :
//         (ray_y - floor(ray_y / BLOCK) * BLOCK) * delta_dist_y;

//     while (!touch(ray_x, ray_y, game))
//     {
//         if (side_dist_x < side_dist_y)
//         {
//             side_dist_x += delta_dist_x;
//             ray_x += step_x;
//             ray_y += step_x * sin_angle / cos_angle;
//             hit_vertical = 1;
//             texture_direction = (cos_angle > 0) ? 0 : 1;
//         }
//         else
//         {
//             side_dist_y += delta_dist_y;
//             ray_y += step_y;
//             ray_x += step_y * cos_angle / sin_angle;
//             hit_vertical = 0;
//             texture_direction = (sin_angle > 0) ? 3 : 2;
//         }
//     }

//     float dist = fixed_dist(player->playerX, player->playerY, ray_x, ray_y, game);
//     dist = fabs(dist); // Ensure the distance is always positive

//     float height = (BLOCK / dist) * (WIDTH / 2);
//     int start_y = (HEIGHT - height) / 2;
//     int end = start_y + height;

//     if (start_y < 0) start_y = 0;
//     if (end >= HEIGHT) end = HEIGHT - 1;

//     for (int y = 0; y < start_y; y++)
//         put_pixel(screen_x, y, game->element->ceiling_color->converted_color, game);

//     while (start_y < end)
//     {
//         int tex_y = (start_y - (HEIGHT - height) / 2) * game->element->texture->height / height;
//         tex_y = tex_y < 0 ? 0 : tex_y;
//         tex_y = tex_y >= game->element->texture->height ? game->element->texture->height - 1 : tex_y;

//         int tex_x = hit_vertical ? ((int)ray_y % BLOCK) : ((int)ray_x % BLOCK);
//         tex_x = (tex_x * game->element->texture->width) / BLOCK;
//         tex_x = tex_x < 0 ? 0 : tex_x;
//         tex_x = tex_x >= game->element->texture->width ? game->element->texture->width - 1 : tex_x;

//         int tex_color = 0;
//         if (texture_direction == 0)
//             tex_color = get_texture_pixel(game->element->texture->east_data, tex_x, tex_y, game->element->texture->size_line, game->element->texture->bpp);
//         else if (texture_direction == 1)
//             tex_color = get_texture_pixel(game->element->texture->west_data, tex_x, tex_y, game->element->texture->size_line, game->element->texture->bpp);
//         else if (texture_direction == 2)
//             tex_color = get_texture_pixel(game->element->texture->north_data, tex_x, tex_y, game->element->texture->size_line, game->element->texture->bpp);
//         else if (texture_direction == 3)
//             tex_color = get_texture_pixel(game->element->texture->south_data, tex_x, tex_y, game->element->texture->size_line, game->element->texture->bpp);

//         put_pixel(screen_x, start_y, 255, game);
//         start_y++;
//     }

//     for (int y = end; y < HEIGHT; y++)
//         put_pixel(screen_x, y, game->element->floor_color->converted_color, game);
// }


// void draw_line(t_player *player, t_game *game, float ray_angle, int screen_x)
// {
//     // Initialize raycasting info
//     t_ray ray;
//     ray.camera_x = 2 * screen_x / (double)WIDTH - 1;
//     ray.dir_x = game->player.player_dirX + game->player.player_planeX * ray.camera_x;
//     ray.dir_y = game->player.player_dirY + game->player.player_planeY * ray.camera_x;

//     ray.map_x = (int)(player->playerX / BLOCK);
//     ray.map_y = (int)(player->playerY / BLOCK);

//     ray.deltadist_x = fabs(1 / ray.dir_x);
//     ray.deltadist_y = fabs(1 / ray.dir_y);

//     // Set step and initial side_dist
//     if (ray.dir_x < 0)
//     {
//         ray.step_x = -1;
//         ray.sidedist_x = (player->playerX / BLOCK - ray.map_x) * ray.deltadist_x;
//     }
//     else
//     {
//         ray.step_x = 1;
//         ray.sidedist_x = (ray.map_x + 1.0 - player->playerX / BLOCK) * ray.deltadist_x;
//     }
//     if (ray.dir_y < 0)
//     {
//         ray.step_y = -1;
//         ray.sidedist_y = (player->playerY / BLOCK - ray.map_y) * ray.deltadist_y;
//     }
//     else
//     {
//         ray.step_y = 1;
//         ray.sidedist_y = (ray.map_y + 1.0 - player->playerY / BLOCK) * ray.deltadist_y;
//     }

//     // Perform DDA
//     int hit = 0;
//     int side;
//     while (!hit)
//     {
//         if (ray.sidedist_x < ray.sidedist_y)
//         {
//             ray.sidedist_x += ray.deltadist_x;
//             ray.map_x += ray.step_x;
//             side = 0; // Hit vertical wall
//         }
//         else
//         {
//             ray.sidedist_y += ray.deltadist_y;
//             ray.map_y += ray.step_y;
//             side = 1; // Hit horizontal wall
//         }
//         // Check if ray hit a wall
//         if (game->map[ray.map_y][ray.map_x] == '1')
//             hit = 1;
//     }

//     // Calculate perpendicular distance to the wall
//     if (side == 0)
//         ray.wall_dist = (ray.map_x - player->playerX / BLOCK + (1 - ray.step_x) / 2) / ray.dir_x;
//     else
//         ray.wall_dist = (ray.map_y - player->playerY / BLOCK + (1 - ray.step_y) / 2) / ray.dir_y;

//     // Calculate wall height and position
//     ray.line_height = (int)(HEIGHT / ray.wall_dist);
//     ray.draw_start = -ray.line_height / 2 + HEIGHT / 2;
//     ray.draw_start = ray.draw_start < 0 ? 0 : ray.draw_start;
//     ray.draw_end = ray.line_height / 2 + HEIGHT / 2;
//     ray.draw_end = ray.draw_end >= HEIGHT ? HEIGHT - 1 : ray.draw_end;

//     // Calculate texture X coordinate
//     double wall_x; // Exact position where the wall was hit
//     if (side == 0)
//         wall_x = player->playerY / BLOCK + ray.wall_dist * ray.dir_y;
//     else
//         wall_x = player->playerX / BLOCK + ray.wall_dist * ray.dir_x;
//     wall_x -= floor(wall_x);

//     int tex_x = (int)(wall_x * (double)(game->element->texture->width));
//     if (side == 0 && ray.dir_x > 0)
//         tex_x = game->element->texture->width - tex_x - 1;
//     if (side == 1 && ray.dir_y < 0)
//         tex_x = game->element->texture->width - tex_x - 1;

//     // Draw the wall
//     for (int y = ray.draw_start; y < ray.draw_end; y++)
//     {
//         int tex_y = (int)(((y - HEIGHT / 2 + ray.line_height / 2) * game->element->texture->height) / ray.line_height);
//         tex_y = tex_y < 0 ? 0 : tex_y;
//         tex_y = tex_y >= game->element->texture->height ? game->element->texture->height - 1 : tex_y;

//         int tex_color;
//         if (side == 0 && ray.dir_x > 0)
//             tex_color = get_texture_pixel(game->element->texture->east_data, tex_x, tex_y, game->element->texture->size_line, game->element->texture->bpp);
//         else if (side == 0 && ray.dir_x < 0)
//             tex_color = get_texture_pixel(game->element->texture->west_data, tex_x, tex_y, game->element->texture->size_line, game->element->texture->bpp);
//         else if (side == 1 && ray.dir_y > 0)
//             tex_color = get_texture_pixel(game->element->texture->south_data, tex_x, tex_y, game->element->texture->size_line, game->element->texture->bpp);
//         else
//             tex_color = get_texture_pixel(game->element->texture->north_data, tex_x, tex_y, game->element->texture->size_line, game->element->texture->bpp);

//         put_pixel(screen_x, y, tex_color, game);
//     }

//     // Draw ceiling and floor
//     for (int y = 0; y < ray.draw_start; y++)
//         put_pixel(screen_x, y, game->element->ceiling_color->converted_color, game);
//     for (int y = ray.draw_end; y < HEIGHT; y++)
//         put_pixel(screen_x, y, game->element->floor_color->converted_color, game);
// }

void draw_line(t_player *player, t_game *game, float ray_angle, int screen_x)
{
    // Ray initialization
    float ray_dir_x = cos(ray_angle);
    float ray_dir_y = sin(ray_angle);

    int map_x = (int)(player->playerX / BLOCK);
    int map_y = (int)(player->playerY / BLOCK);

    float delta_dist_x = fabs(1 / ray_dir_x);
    float delta_dist_y = fabs(1 / ray_dir_y);

    float side_dist_x, side_dist_y;
    int step_x, step_y;

    // Calculate step and initial side_dist
    if (ray_dir_x < 0)
    {
        step_x = -1;
        side_dist_x = (player->playerX / BLOCK - map_x) * delta_dist_x;
    }
    else
    {
        step_x = 1;
        side_dist_x = (map_x + 1.0 - player->playerX / BLOCK) * delta_dist_x;
    }
    if (ray_dir_y < 0)
    {
        step_y = -1;
        side_dist_y = (player->playerY / BLOCK - map_y) * delta_dist_y;
    }
    else
    {
        step_y = 1;
        side_dist_y = (map_y + 1.0 - player->playerY / BLOCK) * delta_dist_y;
    }

    // Perform DDA
    int hit = 0;
    int side; // 0: vertical wall, 1: horizontal wall
    while (!hit)
    {
        if (side_dist_x < side_dist_y)
        {
            side_dist_x += delta_dist_x;
            map_x += step_x;
            side = 0; // Vertical wall
        }
        else
        {
            side_dist_y += delta_dist_y;
            map_y += step_y;
            side = 1; // Horizontal wall
        }

        // Check for wall hit
        if (game->map[map_y][map_x] == '1')
            hit = 1;
    }

    // Calculate perpendicular distance to the wall
    float perp_wall_dist;
    if (side == 0)
        perp_wall_dist = (map_x - player->playerX / BLOCK + (1 - step_x) / 2) / ray_dir_x;
    else
        perp_wall_dist = (map_y - player->playerY / BLOCK + (1 - step_y) / 2) / ray_dir_y;

    // Correct for fish-eye effect
    perp_wall_dist *= cos(ray_angle - player->angle);

    // Calculate line height and rendering range
    int line_height = (int)(HEIGHT / perp_wall_dist);
    int draw_start = -line_height / 2 + HEIGHT / 2;
    if (draw_start < 0)
        draw_start = 0;
    int draw_end = line_height / 2 + HEIGHT / 2;
    if (draw_end >= HEIGHT)
        draw_end = HEIGHT - 1;

    // Calculate wall hit position for texture mapping
    float wall_x;
    if (side == 0)
        wall_x = player->playerY / BLOCK + perp_wall_dist * ray_dir_y;
    else
        wall_x = player->playerX / BLOCK + perp_wall_dist * ray_dir_x;
    wall_x -= floor(wall_x);

    int tex_x = (int)(wall_x * game->element->texture->width);
    if (side == 0 && ray_dir_x > 0)
        tex_x = game->element->texture->width - tex_x - 1;
    if (side == 1 && ray_dir_y < 0)
        tex_x = game->element->texture->width - tex_x - 1;

    // Draw the wall slice
    for (int y = draw_start; y < draw_end; y++)
    {
        int tex_y = (int)(((y - HEIGHT / 2 + line_height / 2) * game->element->texture->height) / line_height);
        tex_y = tex_y < 0 ? 0 : tex_y;
        tex_y = tex_y >= game->element->texture->height ? game->element->texture->height - 1 : tex_y;

        int tex_color;
        if (side == 0 && ray_dir_x > 0)
            tex_color = get_texture_pixel(game->element->texture->east_data, tex_x, tex_y, game->element->texture->size_line, game->element->texture->bpp);
        else if (side == 0 && ray_dir_x < 0)
            tex_color = get_texture_pixel(game->element->texture->west_data, tex_x, tex_y, game->element->texture->size_line, game->element->texture->bpp);
        else if (side == 1 && ray_dir_y > 0)
            tex_color = get_texture_pixel(game->element->texture->south_data, tex_x, tex_y, game->element->texture->size_line, game->element->texture->bpp);
        else
            tex_color = get_texture_pixel(game->element->texture->north_data, tex_x, tex_y, game->element->texture->size_line, game->element->texture->bpp);

        put_pixel(screen_x, y, tex_color, game);
    }

    // Draw ceiling
    for (int y = 0; y < draw_start; y++)
        put_pixel(screen_x, y, game->element->ceiling_color->converted_color, game);

    // Draw floor
    for (int y = draw_end; y < HEIGHT; y++)
        put_pixel(screen_x, y, game->element->floor_color->converted_color, game);
}


// void draw_line(t_player *player, t_game *game, float ray_angle, int screen_x)
// {
//     // Ray initialization
//     float ray_dir_x = cos(ray_angle);
//     float ray_dir_y = sin(ray_angle);

//     int map_x = (int)(player->playerX / BLOCK);
//     int map_y = (int)(player->playerY / BLOCK);

//     float delta_dist_x = fabs(1 / ray_dir_x);
//     float delta_dist_y = fabs(1 / ray_dir_y);

//     float side_dist_x, side_dist_y;
//     int step_x, step_y;

//     // Calculate step and initial side_dist
//     if (ray_dir_x < 0)
//     {
//         step_x = -1;
//         side_dist_x = (player->playerX / BLOCK - map_x) * delta_dist_x;
//     }
//     else
//     {
//         step_x = 1;
//         side_dist_x = (map_x + 1.0 - player->playerX / BLOCK) * delta_dist_x;
//     }
//     if (ray_dir_y < 0)
//     {
//         step_y = -1;
//         side_dist_y = (player->playerY / BLOCK - map_y) * delta_dist_y;
//     }
//     else
//     {
//         step_y = 1;
//         side_dist_y = (map_y + 1.0 - player->playerY / BLOCK) * delta_dist_y;
//     }

//     // Perform DDA
//     int hit = 0;
//     int side; // 0: vertical wall, 1: horizontal wall
//     while (!hit)
//     {
//         if (side_dist_x < side_dist_y)
//         {
//             side_dist_x += delta_dist_x;
//             map_x += step_x;
//             side = 0; // Vertical wall
//         }
//         else
//         {
//             side_dist_y += delta_dist_y;
//             map_y += step_y;
//             side = 1; // Horizontal wall
//         }

//         // Check for wall hit
//         if (game->map[map_y][map_x] == '1')
//             hit = 1;
//     }

//     // Calculate perpendicular distance to the wall
//     float perp_wall_dist;
//     if (side == 0)
//         perp_wall_dist = (map_x - player->playerX / BLOCK + (1 - step_x) / 2) / ray_dir_x;
//     else
//         perp_wall_dist = (map_y - player->playerY / BLOCK + (1 - step_y) / 2) / ray_dir_y;

//     // Calculate line height and rendering range
//     int line_height = (int)(HEIGHT / perp_wall_dist);
//     int draw_start = -line_height / 2 + HEIGHT / 2;
//     if (draw_start < 0)
//         draw_start = 0;
//     int draw_end = line_height / 2 + HEIGHT / 2;
//     if (draw_end >= HEIGHT)
//         draw_end = HEIGHT - 1;

//     // Calculate wall hit position for texture mapping
//     float wall_x;
//     if (side == 0)
//         wall_x = player->playerY / BLOCK + perp_wall_dist * ray_dir_y;
//     else
//         wall_x = player->playerX / BLOCK + perp_wall_dist * ray_dir_x;
//     wall_x -= floor(wall_x);

//     // Calculate texture X coordinate
//     int tex_x = (int)(wall_x * game->element->texture->width);
//     if (side == 0 && ray_dir_x > 0)
//         tex_x = game->element->texture->width - tex_x - 1;
//     if (side == 1 && ray_dir_y < 0)
//         tex_x = game->element->texture->width - tex_x - 1;

//     // Draw the wall slice
//     for (int y = draw_start; y < draw_end; y++)
//     {
//         // Calculate texture Y coordinate
//         int tex_y = (int)(((y - HEIGHT / 2 + line_height / 2) * game->element->texture->height) / line_height);
//         tex_y = tex_y < 0 ? 0 : tex_y;
//         tex_y = tex_y >= game->element->texture->height ? game->element->texture->height - 1 : tex_y;

//         // Fetch texture pixel color
//         int tex_color;
//         if (side == 0 && ray_dir_x > 0)
//             tex_color = get_texture_pixel(game->element->texture->east_data, tex_x, tex_y, game->element->texture->size_line, game->element->texture->bpp);
//         else if (side == 0 && ray_dir_x < 0)
//             tex_color = get_texture_pixel(game->element->texture->west_data, tex_x, tex_y, game->element->texture->size_line, game->element->texture->bpp);
//         else if (side == 1 && ray_dir_y > 0)
//             tex_color = get_texture_pixel(game->element->texture->south_data, tex_x, tex_y, game->element->texture->size_line, game->element->texture->bpp);
//         else
//             tex_color = get_texture_pixel(game->element->texture->north_data, tex_x, tex_y, game->element->texture->size_line, game->element->texture->bpp);

//         // Draw the pixel on the screen
//         put_pixel(screen_x, y, tex_color, game);
//     }

//     // Draw ceiling
//     for (int y = 0; y < draw_start; y++)
//         put_pixel(screen_x, y, game->element->ceiling_color->converted_color, game);

//     // Draw floor
//     for (int y = draw_end; y < HEIGHT; y++)
//         put_pixel(screen_x, y, game->element->floor_color->converted_color, game);
// }



int draw_loop(t_game *game)
{
    t_player *player = &game->player;
    move_player(player, game);
    clear_image(game);

    if (DEBUG)
    {
        draw_square(player->playerX, player->playerY, 10, 0x00FFFF, game);
        draw_map(game);
    }

    float fraction = PI / 3 / WIDTH;
    float start_x = player->angle - PI / 6;
    int i = 0;

    while (i < WIDTH)
    {
        draw_line(player, game, start_x, i);
        start_x += fraction;
        i++;
    }

    mlx_put_image_to_window(game->mlx, game->win, game->img, 0, 0);
    return 0;
}

