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

void	free_from_index(t_fdf_data *d, int i);

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

int	parse_map_file(const char *file_name, t_fdf_data *d)
{
	char	*line;
	int		fd;
	int		i;

	d->rows = count_rows(file_name);
	d->fd = open(file_name, O_RDONLY);
	line = get_next_line(fd);
	if (d->rows <= 0 || fd < 0 || !line)
		return (FAILURE);
	d->cols = word_count(line);
	if (create_matrix_elements(d) == FAILURE)
		return (free_ptr_return_int((void *)&line, FAILURE));
	i = -1;
	while (++i < d->rows)
	{
		if (!line)
		{
			free_matrix_elements(m);
			return (FAILURE);
		}
		fill_matrix_row(m, i, line);
		free(line);
		line = get_next_line(fd);
	}
	close(fd);
	return (SUCCESS);
}

int	allocate_data(t_fdf_data *d)
{
	int	i;

	d->map = ft_calloc(d->rows, sizeof(int *));
	d->colors = ft_calloc(d->rows, sizeof(unsigned int *));
	d->world = ft_calloc(d->rows, sizeof(t_dp3 *));
	d->view = ft_calloc(d->rows, sizeof(t_dp2 *));
	d->pixels = ft_calloc(d->rows, sizeof(t_ip2 *));
	if (!d->map || !d->colors || !d->world || !d->view || !d->pixels)
		return (FAILURE);
	i = -1;
	while (++i < d->rows)
	{
		d->map[i] = ft_calloc(d->cols, sizeof(int));
		d->colors[i] = ft_calloc(d->cols, sizeof(unsigned int));
		d->world[i] = ft_calloc(d->cols, sizeof(t_dp3));
		d->view[i] = ft_calloc(d->cols, sizeof(t_dp2));
		d->pixels[i] = ft_calloc(d->cols, sizeof(t_ip2));
		if (!d->map[i] || !d->colors[i] || !d->world[i]
			|| !d->view[i] || !d->pixels[i])
		{
			free_data(d);
			return (FAILURE);
		}
	}
}

void	free_from_index(t_fdf_data *d, int i)
{
	int	i;
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
