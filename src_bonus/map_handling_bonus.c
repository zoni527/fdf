/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_handling_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvarila <jvarila@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 13:25:32 by jvarila           #+#    #+#             */
/*   Updated: 2025/01/29 11:12:25 by jvarila          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/fdf.h"

static void	assign_map_and_set_pixel_colors(t_fdf *data);
static void	assign_row(t_fdf *data, int row, char **words);
static int	count_rows_and_columns(t_fdf *data, char *line);

int	validate_map_file(const char *file_name, t_fdf *data)
{
	char	*line;

	if (!ft_strrchr(file_name, '.')
		|| ft_strlen(ft_strrchr(file_name, '.')) != 4
		|| !ft_strnstr(ft_strrchr(file_name, '.'), ".fdf", 4))
		return (FAILURE);
	data->fd = open(file_name, O_RDONLY);
	line = get_next_line(data->fd);
	if (data->fd < 0 || !line)
		return (FAILURE);
	if (count_rows_and_columns(data, line) == FAILURE)
		return (FAILURE);
	close(data->fd);
	data->fd = -1;
	return (SUCCESS);
}

void	parse_map_file(const char *file_name, t_fdf *data)
{
	data->fd = open(file_name, O_RDONLY);
	if (data->fd < 0)
		free_close_print_exit(data, "ERROR: couldn't open file");
	assign_map_and_set_pixel_colors(data);
	close(data->fd);
	data->fd = -1;
}

static int	count_rows_and_columns(t_fdf *data, char *line)
{
	int	wc_o;
	int	wc;

	wc_o = word_count(line);
	if (!wc_o)
	{
		free(line);
		return (FAILURE);
	}
	while (line)
	{
		data->rows++;
		wc = word_count(line);
		if (wc > wc_o)
		{
			free(line);
			return (FAILURE);
		}
		free(line);
		line = get_next_line(data->fd);
	}
	data->cols = wc_o;
	return (SUCCESS);
}

static void	assign_map_and_set_pixel_colors(t_fdf *data)
{
	char	*line;
	char	**words;
	int		i;

	i = -1;
	while (++i < data->rows)
	{
		line = get_next_line(data->fd);
		if (!line)
			free_close_print_exit(data, "ERROR: malloc fail in get_next_line");
		words = ft_split(line, ' ');
		if (!words)
		{
			free(line);
			free_close_print_exit(data, "ERROR: malloc fail in split");
		}
		assign_row(data, i, words);
		free_split(&words);
		free(line);
	}
}

static void	assign_row(t_fdf *data, int row, char **words)
{
	int				j;
	unsigned int	rgba;

	j = -1;
	while (++j < data->cols)
	{
		if (!words[j])
			break ;
		data->map[row][j] = ft_atoi(words[j]);
		if (ft_strchr(words[j], ','))
			rgba = atohex(ft_strchr(words[j], ',') + 1) << 8 | 0xff;
		else
			rgba = WHITE;
		data->pixels[row][j].rgba = rgba;
	}
	while (j < data->cols)
	{
		data->map[row][j] = 0;
		data->pixels[row][j].rgba = WHITE;
		j++;
	}
}
