/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   world_max_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvarila <jvarila@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 12:47:05 by jvarila           #+#    #+#             */
/*   Updated: 2025/01/29 11:13:00 by jvarila          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/fdf.h"

double	max_x_world(t_fdf *data)
{
	double	max_x;
	size_t	i;

	max_x = data->world[0][0].x;
	i = 1;
	while (i < (size_t)(data->cols * data->rows))
	{
		if (data->world[i / data->cols][i % data->cols].x > max_x)
			max_x = data->world[i / data->cols][i % data->cols].x;
		i++;
	}
	return (max_x);
}

double	max_y_world(t_fdf *data)
{
	double	max_y;
	size_t	i;

	max_y = data->world[0][0].y;
	i = 1;
	while (i < (size_t)(data->cols * data->rows))
	{
		if (data->world[i / data->cols][i % data->cols].y > max_y)
			max_y = data->world[i / data->cols][i % data->cols].y;
		i++;
	}
	return (max_y);
}

double	max_z_world(t_fdf *data)
{
	double	max_z;
	size_t	i;

	max_z = data->world[0][0].z;
	i = 1;
	while (i < (size_t)(data->cols * data->rows))
	{
		if (data->world[i / data->cols][i % data->cols].z > max_z)
			max_z = data->world[i / data->cols][i % data->cols].z;
		i++;
	}
	return (max_z);
}
