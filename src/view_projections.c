/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   view_projections.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvarila <jvarila@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 12:48:08 by jvarila           #+#    #+#             */
/*   Updated: 2025/01/23 12:48:35 by jvarila          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/fdf.h"

void	y_plane_projection(t_fdf *data)
{
	int	i;
	int	j;

	i = -1;
	while (++i < data->rows)
	{
		j = -1;
		while (++j < data->cols)
		{
			data->view[i][j].x = data->world[i][j].x;
			data->view[i][j].y = data->world[i][j].z;
		}
	}
}

void	x_plane_projection(t_fdf *data)
{
	int	i;
	int	j;

	i = -1;
	while (++i < data->rows)
	{
		j = -1;
		while (++j < data->cols)
		{
			data->view[i][j].x = data->world[i][j].y;
			data->view[i][j].y = data->world[i][j].z;
		}
	}
}

void	z_plane_projection(t_fdf *data)
{
	int	i;
	int	j;

	i = -1;
	while (++i < data->rows)
	{
		j = -1;
		while (++j < data->cols)
		{
			data->view[i][j].x = data->world[i][j].x;
			data->view[i][j].y = data->world[i][j].y;
		}
	}
}
