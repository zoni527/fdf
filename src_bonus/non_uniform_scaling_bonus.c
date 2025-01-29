/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   non_uniform_scaling_bonus.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvarila <jvarila@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 14:01:09 by jvarila           #+#    #+#             */
/*   Updated: 2025/01/29 11:12:30 by jvarila          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/fdf.h"

void	scale_world_x(t_fdf *data, double multiplier)
{
	int	i;
	int	j;

	i = -1;
	while (++i < data->rows)
	{
		j = -1;
		while (++j < data->cols)
			data->world[i][j].x *= multiplier;
	}
}

void	scale_world_y(t_fdf *data, double multiplier)
{
	int	i;
	int	j;

	i = -1;
	while (++i < data->rows)
	{
		j = -1;
		while (++j < data->cols)
			data->world[i][j].y *= multiplier;
	}
}

void	scale_world_z(t_fdf *data, double multiplier)
{
	int	i;
	int	j;

	i = -1;
	while (++i < data->rows)
	{
		j = -1;
		while (++j < data->cols)
			data->world[i][j].z *= multiplier;
	}
}
