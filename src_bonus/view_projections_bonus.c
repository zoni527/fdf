/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   view_projections_bonus.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvarila <jvarila@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 12:48:08 by jvarila           #+#    #+#             */
/*   Updated: 2025/01/29 11:12:52 by jvarila          ###   ########.fr       */
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

void	oblique_projection(t_fdf *data)
{
	int		i;
	int		j;
	double	x;
	double	y;
	double	z;

	i = -1;
	while (++i < data->rows)
	{
		j = -1;
		while (++j < data->cols)
		{
			x = data->world[i][j].x;
			y = data->world[i][j].y;
			z = data->world[i][j].z;
			data->view[i][j].x = x + 2.0 * y * cos(60 * ONE_DEGREE);
			data->view[i][j].y = z + 2.0 * y * sin(60 * ONE_DEGREE);
		}
	}
}
