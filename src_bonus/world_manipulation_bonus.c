/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   world_manipulation_bonus.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvarila <jvarila@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 12:56:35 by jvarila           #+#    #+#             */
/*   Updated: 2025/01/29 11:12:58 by jvarila          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/fdf.h"

void	assign_world(t_fdf *data)
{
	int	i;
	int	j;

	i = -1;
	while (++i < data->rows)
	{
		j = -1;
		while (++j < data->cols)
		{
			data->world[i][j].x = j;
			data->world[i][j].y = i;
			data->world[i][j].z = data->map[i][j];
		}
	}
}

void	offset_world(t_fdf *data, t_dp3 offset)
{
	int	i;
	int	j;

	i = -1;
	while (++i < data->rows)
	{
		j = -1;
		while (++j < data->cols)
			offset_dp3(&data->world[i][j], offset);
	}
}

void	scale_world(t_fdf *data, double multiplier)
{
	int	i;
	int	j;

	i = -1;
	while (++i < data->rows)
	{
		j = -1;
		while (++j < data->cols)
			scale_dp3(&data->world[i][j], multiplier);
	}
}

void	rotate_world(t_fdf *data, double rx, double ry, double rz)
{
	int	i;
	int	j;

	i = -1;
	while (++i < data->rows)
	{
		j = -1;
		while (++j < data->cols)
			rotate_dp3(&data->world[i][j], rx, ry, rz);
	}
}
