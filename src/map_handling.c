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
		if (!valid_multinumberstring(line))
			return (free_ptr_return_int((void *)&line, FAILURE));
		wc = word_count(line);
		if (wc != wc_o)
			return (free_ptr_return_int((void *)&line, FAILURE));
		free(line);
		line = get_next_line(fd);
	}
	return (SUCCESS);
}

int	parse_map_file(const char *file_name, t_matrix *m)
{
	char	*line;
	int		fd;
	int		i;

	m->rows = count_rows(file_name);
	fd = open(file_name, O_RDONLY);
	line = get_next_line(fd);
	if (m->rows <= 0 || fd < 0 || !line)
		return (FAILURE);
	m->cols = word_count(line);
	if (create_matrix_elements(m) == FAILURE)
		return (free_ptr_return_int((void *)&line, FAILURE));
	i = -1;
	while (++i < m->rows)
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
	return (SUCCESS);
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
