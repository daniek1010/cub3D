/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils1.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ojacobs <ojacobs@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 20:13:41 by danevans          #+#    #+#             */
/*   Updated: 2024/12/05 21:02:52 by ojacobs          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./parsing/parsing.h"

char	*ft_trim_newline(char *read_file)
{
	int		len;
	char	*trim_str;

	len = ft_strlen(read_file);
	if (read_file[len - 1] == '\n')
	{
		trim_str = malloc(sizeof(char) * len);
		ft_strlcpy(trim_str, read_file, len);
		return (trim_str);
	}
	return (read_file);
}

int	valid_extension_file_check(char *argv, int ac, char *str)
{
	char	*file;
	int		len;

	if (ac == 2)
	{
		file = argv;
		len = ft_strlen(file) - 4;
		file += len;
		if (*file == '.')
		{
			while (*file)
			{
				file++;
				if ((!ft_strncmp(file, str, 3)) && (ft_strlen(file) == 3))
					return (1);
			}
		}
		ft_error("invalid extension\n");
		return (0);
	}
	ft_error("invalid number of args\n");
	return (0);
}

int	valid_extension_args_no(char *argv, int ac, char *str)
{
	char	*file;

	if (ac == 2)
	{
		file = argv;
		while (*file)
		{
			if (*file == '.')
			{
				file++;
				if (*file == '/')
					continue ;
				if ((!ft_strncmp(file, str, 3)) && (ft_strlen(file) == 3))
					return (1);
				ft_error("invalid extension\n");
				return (0);
			}
			file++;
		}
		ft_error("invalid extension\n");
		return (0);
	}
	ft_error("invalid number of args\n");
	return (0);
}

char	*ft_skip_check_element_char(char *file)
{
	char	*trimmed_str;

	trimmed_str = file;
	while (*trimmed_str == ' ' || *trimmed_str == 9
		|| (*trimmed_str >= 11 && *trimmed_str <= 13))
		trimmed_str++;
	if (*trimmed_str == 'N' || *trimmed_str == 'S' || *trimmed_str == 'W'
		|| *trimmed_str == 'E' || *trimmed_str == 'F' || *trimmed_str == 'C'
		|| *trimmed_str == '1')
		return (trimmed_str);
	if (*trimmed_str == '\n')
		return (trimmed_str);
	return (NULL);
}

void	ft_error(char *msg_error)
{
	while (*msg_error)
		write (1, msg_error++, 1);
}

// void	ft_close_error(char *str, int fd)
// {
// 	ft_error(str);
// 	close(fd);
// }
