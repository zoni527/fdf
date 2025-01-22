/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_handling.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvarila <jvarila@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 13:25:32 by jvarila           #+#    #+#             */
/*   Updated: 2025/01/15 13:37:31 by jvarila          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/fdf.h"

void	allocate_data(t_fdf_data *d);
void	assign_map_and_set_pixel_colors(t_fdf_data *data);
void	assign_row(t_fdf_data *data, int row, char **words);
int		count_rows_and_columns(t_fdf_data *data, char *line);

int	validate_map_file(const char *file_name, t_fdf_data *data)
{
	char	*line;

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

int	count_rows_and_columns(t_fdf_data *data, char *line)
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
		if (wc != wc_o)
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

void	parse_map_file(const char *file_name, t_fdf_data *data)
{
	data->fd = open(file_name, O_RDONLY);
	if (data->fd < 0)
		free_close_print_exit(data, "ERROR: couldn't open file");
	allocate_data(data);
	assign_map_and_set_pixel_colors(data);
	close(data->fd);
	data->fd = -1;
}

void	assign_map_and_set_pixel_colors(t_fdf_data *data)
{
	char	*line;
	char	**words;
	int		i;
	int		j;

	i = -1;
	while (++i < data->rows)
	{
		line = get_next_line(data->fd);
		words = ft_split(line, ' ');
		if (!words)
		{
			free(line);
			free_close_print_exit(data, "ERROR: malloc fail in split");
		}
		assign_row(data, i, words);
		j = -1;
		while (++j < data->cols)
			free(words[j]);
		free(words);
		free(line);
	}
}

void	assign_row(t_fdf_data *data, int row, char **words)
{
	int				j;
	unsigned int	rgba;

	j = -1;
	while (++j < data->cols)
	{
		data->map[row][j] = ft_atoi(words[j]);
		if (ft_strchr(words[j], ','))
			rgba = atohex(ft_strchr(words[j], ',') + 1) << 8 | 0xff;
		else
			rgba = WHITE;
		data->pixels[row][j].rgba = rgba;
	}
}

void	allocate_data(t_fdf_data *d)
{
	int	i;

	d->map = ft_calloc(d->rows, sizeof(int *));
	d->world = ft_calloc(d->rows, sizeof(t_dp3 *));
	d->view = ft_calloc(d->rows, sizeof(t_dp2 *));
	d->pixels = ft_calloc(d->rows, sizeof(t_pixel *));
	if (!d->map || !d->world || !d->view || !d->pixels)
		free_close_print_exit(d, "ERROR: couldn't allocate data");
	i = -1;
	while (++i < d->rows)
	{;
		d->map[i] = ft_calloc(d->cols, sizeof(int));
		d->world[i] = ft_calloc(d->cols, sizeof(t_dp3));
		d->view[i] = ft_calloc(d->cols, sizeof(t_dp2));
		d->pixels[i] = ft_calloc(d->cols, sizeof(t_pixel));
		if (!d->map[i] || !d->world[i] || !d->view[i] || !d->pixels[i])
			free_close_print_exit(d, "ERROR: couldn't allocate data");
	}
}
