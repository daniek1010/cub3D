/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils0.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danevans <danevans@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 23:45:41 by danevans          #+#    #+#             */
/*   Updated: 2024/11/07 21:47:18 by danevans         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int	ft_open_file(char *read_file)
{
	int	fd;

	fd = open(read_file, O_RDONLY);
	if (fd == -1)
	{
		ft_error("OPEN failed\n");
		return (0);
	}
	return (fd);
}

void	ft_free_trimmed_line_read(char *trim_line, char *line_read)
{
	if (line_read)
		free (line_read);
	if (trim_line)
		free (trim_line);
}

int	validating_texture(char *trim_file, t_parser *element)
{
	if ((ft_strncmp(trim_file, "WE", 2) == 0)
		|| (ft_strncmp(trim_file, "EA", 2) == 0)
		|| (ft_strncmp(trim_file, "NO", 2) == 0)
		|| (ft_strncmp(trim_file, "SO", 2) == 0))
	{
		return (1);
	}
	return (0);
}

int	validating_ceiling_floor(char *trim_file, t_parser *element)
{
	char	*trim_newline;

	if (ft_strncmp(trim_file, "C", 1) == 0)
	{
		trim_file++;
		trim_file = ft_iswhitespace(trim_file);
		trim_newline = ft_trim_newline(trim_file);
		if (saving_ceiling_and_floor(trim_newline, element->ceiling_color)){
			free(trim_newline);
			return (1);
		}
	}
	else if (ft_strncmp(trim_file, "F", 1) == 0)
	{
		trim_file++;
		trim_file = ft_iswhitespace(trim_file);
		trim_newline = ft_trim_newline(trim_file);
		if (saving_ceiling_and_floor(trim_newline, element->floor_color)){
			free(trim_newline);
			return (1);
		}
	}
	ft_error("Invalid color passed");
	free(trim_newline);
	return (0);
}

char	*ft_iswhitespace(char *readfile)
{
	while (*readfile)
	{
		if (*readfile == ' ' || (*readfile >= 9 && *readfile <= 13))
			readfile++;
		else
			break ;
	}
	return ((char *)readfile);
}
