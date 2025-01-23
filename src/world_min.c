/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   world_min.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvarila <jvarila@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 12:45:06 by jvarila           #+#    #+#             */
/*   Updated: 2025/01/23 12:46:14 by jvarila          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/fdf.h"

double	min_x_world(t_fdf *data)
{
	double	min_x;
	size_t	i;

	min_x = data->world[0][0].x;
	i = 1;
	while (i < (size_t)(data->cols * data->rows))
	{
		if (data->world[i / data->cols][i % data->cols].x < min_x)
			min_x = data->world[i / data->cols][i % data->cols].x;
		i++;
	}
	return (min_x);
}

double	min_y_world(t_fdf *data)
{
	double	min_y;
	size_t	i;

	min_y = data->world[0][0].y;
	i = 1;
	while (i < (size_t)(data->cols * data->rows))
	{
		if (data->world[i / data->cols][i % data->cols].y < min_y)
			min_y = data->world[i / data->cols][i % data->cols].y;
		i++;
	}
	return (min_y);
}

double	min_z_world(t_fdf *data)
{
	double	min_z;
	size_t	i;

	min_z = data->world[0][0].z;
	i = 1;
	while (i < (size_t)(data->cols * data->rows))
	{
		if (data->world[i / data->cols][i % data->cols].z < min_z)
			min_z = data->world[i / data->cols][i % data->cols].z;
		i++;
	}
	return (min_z);
}
