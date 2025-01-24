/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   view_centering.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvarila <jvarila@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 12:35:51 by jvarila           #+#    #+#             */
/*   Updated: 2025/01/23 12:38:15 by jvarila          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/fdf.h"

void	center_view(t_fdf *data)
{
	center_view_x(data);
	center_view_y(data);
}

void	center_view_x(t_fdf *data)
{
	double	min_x;
	double	max_x;
	int		i;
	int		j;
	t_dp2	offset;

	min_x = data->view[0][0].x;
	max_x = data->view[0][0].x;
	i = -1;
	while (++i < data->rows)
	{
		j = -1;
		while (++j < data->cols)
		{
			if (data->view[i][j].x < min_x)
				min_x = data->view[i][j].x;
			if (data->view[i][j].x > max_x)
				max_x = data->view[i][j].x;
		}
	}
	offset.x = -(max_x + min_x) / 2;
	offset.y = 0;
	data->view_offset.x = offset.x;
	offset_view(data, offset);
}

void	center_view_y(t_fdf *data)
{
	double	min_y;
	double	max_y;
	int		i;
	int		j;
	t_dp2	offset;

	min_y = data->view[0][0].y;
	max_y = data->view[0][0].y;
	i = -1;
	while (++i < data->rows)
	{
		j = -1;
		while (++j < data->cols)
		{
			if (data->view[i][j].y < min_y)
				min_y = data->view[i][j].y;
			if (data->view[i][j].y > max_y)
				max_y = data->view[i][j].y;
		}
	}
	offset.y = -(max_y + min_y) / 2;
	offset.x = 0;
	data->view_offset.y = offset.y;
	offset_view(data, offset);
}
