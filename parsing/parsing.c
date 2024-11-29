/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danevans <danevans@student.42.f>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 23:21:26 by danevans          #+#    #+#             */
/*   Updated: 2024/11/27 19:02:52 by danevans         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int	readfile_and_save_content(char *read_file, t_parser *element)
{
	int		fd;
	char	*line_read;
	char	*trim_file;
	int x;
	x = 0;

	fd = ft_open_file(read_file);
	if (fd == 0)
		return (0);
	while ((line_read = get_next_line(fd)) != NULL)
	{
		trim_file = ft_skip_check_element_char(line_read);
		if (trim_file == NULL)
			break ;
		if (trim_file[0] == '\n')
			continue ;
		if (validating_texture(trim_file, element))
		{
			if (!checking_texture(trim_file, element))
			{
				free(trim_file);
				return (0);
			}
			x = 1;
		}
		else if (trim_file[0] == 'C' || trim_file[0] == 'F')
		{
			if (!validating_ceiling_floor(trim_file, element))
			{
				free(trim_file);
				// free(line_read);
				return (0);
				// break;
			}
			x = 1;
		}
		else if (trim_file[0] == '1')
		{
			if (!validating_map(trim_file, element))
			{	
				ft_error("here hherehhere Invalid elemenT recieved\n");
				// free(line_read);
				free(trim_file);
				return (0);
				// break;
			}
			x = 1;
		}
		// else
		// 	break ;
		// ft_free_trimmed_line_read(trim_file, line_read);	
	}
	free(line_read);
	if (x)
		return (1);
	ft_error("Invalid elemenT recieved\n");
	// ft_free_trimmed_line_read(trim_file, line_read);
	return (0);
}

t_parser	*parsing_func(char *read_file)
{
	t_parser	*element;

	element = init_elements();
	if (element == NULL)
		return (NULL);
	printf("1 .....successffully got here\n\n\n");
	if (!readfile_and_save_content(read_file, element))
	{
		// free(read_file);
		free_parser_struct(element);
		printf("\nbad here\n\n\n");
		return (NULL);
	}
	printf("2.....successffully got here\n\n\n");
	if (!verify_map_walls(element))
	{
		printf("errror here\n\n\n");
		free_parser_struct(element);
		return (NULL);
	}
	printf("3 ...successffully got here\n\n\n");
	return (element);
}

