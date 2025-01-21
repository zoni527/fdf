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
void	assign_map_and_colors(t_fdf_data *data);

int	validate_map_file(const char *file_name)
{
	int		wc_o;
	int		wc;
	int		fd;
	char	*line;

	fd = open(file_name, O_RDONLY);
	line = get_next_line(fd);
	if (fd < 0 || !line)
		return (FAILURE);
	wc_o = word_count(line);
	if (!wc_o)
		return (FAILURE);
	while (line)
	{
		wc = word_count(line);
		if (wc != wc_o)
			return (free_ptr_return_int((void *)&line, FAILURE));
		free(line);
		line = get_next_line(fd);
	}
	close(fd);
	return (SUCCESS);
}

void	parse_map_file(const char *file_name, t_fdf_data *data)
{
	char	*line;

	data->rows = count_rows(file_name);
	data->fd = open(file_name, O_RDONLY);
	if (data->rows <= 0 || data->fd < 0)
		free_print_exit(data, "ERROR: invalid file name or data");
	line = get_next_line(data->fd);
	data->cols = word_count(line);
	while (line)
		line = get_next_line(data->fd);
	close(data->fd);
	data->fd = open(file_name, O_RDONLY);
	allocate_data(data);
	assign_map_and_colors(data);
	close(data->fd);
}

void	assign_map_and_colors(t_fdf_data *data)
{
	char	*line;
	char	*temp;
	int		i;
	int		j;

	i = -1;
	while (++i < data->rows)
	{
		line = get_next_line(data->fd);
		if (!line)
			free_print_exit(data, "ERROR: couldn't read from file");
		temp = line;
		j = -1;
		while (++j < data->cols)
		{
			data->map[i][j] = ft_atoi(temp);
			if (ft_strchr(temp, ','))
				data->colors[i][j] = atohex(ft_strchr(temp, ','));
			else
				data->colors[i][j] = WHITE;
			temp = ft_strchr(temp, ' ');
		}
		free(line);
	}
}

void	allocate_data(t_fdf_data *d)
{
	int	i;

	d->map = ft_calloc(d->rows, sizeof(int *));
	d->colors = ft_calloc(d->rows, sizeof(unsigned int *));
	d->world = ft_calloc(d->rows, sizeof(t_dp3 *));
	d->view = ft_calloc(d->rows, sizeof(t_dp2 *));
	d->pixels = ft_calloc(d->rows, sizeof(t_ip2 *));
	if (!d->map || !d->colors || !d->world || !d->view || !d->pixels)
		free_print_exit(d, "ERROR: couldn't allocate data");
	i = -1;
	while (++i < d->rows)
	{;
		d->map[i] = ft_calloc(d->cols, sizeof(int));
		d->colors[i] = ft_calloc(d->cols, sizeof(unsigned int));
		d->world[i] = ft_calloc(d->cols, sizeof(t_dp3));
		d->view[i] = ft_calloc(d->cols, sizeof(t_dp2));
		d->pixels[i] = ft_calloc(d->cols, sizeof(t_ip2));
		if (!d->map[i] || !d->colors[i] || !d->world[i]
			|| !d->view[i] || !d->pixels[i])
			free_print_exit(d, "ERROR: couldn't allocate data");
	}
}

int	count_rows(const char *file_name)
{
	int		rows;
	int		fd;
	char	*line;

	fd = open(file_name, O_RDONLY);
	if (fd < 0)
		return (-1);
	rows = 0;
	line = get_next_line(fd);
	while (line)
	{
		rows++;
		free(line);
		line = get_next_line(fd);
	}
	close(fd);
	return (rows);
}
