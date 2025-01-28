/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   default_gradient.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvarila <jvarila@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 17:02:27 by jvarila           #+#    #+#             */
/*   Updated: 2025/01/28 09:22:32 by jvarila          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/fdf.h"

void	calculate_channels(t_color *c);

void	set_default_gradient(t_fdf *data)
{
	double	min_z;
	double	max_z;
	t_color	c1;
	t_color	c2;
	t_dp3	k;

	c1.rgba = LINE_GRADIENT_COLOR_1;
	c2.rgba = LINE_GRADIENT_COLOR_2;
	calculate_channels(&c1);
	calculate_channels(&c2);
	min_z = min_z_world(data);
	max_z = max_z_world(data);
	k.x = ((double)c2.r - c1.r) / ((double)max_z - min_z);
	k.y = ((double)c2.g - c1.g) / ((double)max_z - min_z);
	k.z = ((double)c2.b - c1.b) / ((double)max_z - min_z);
	interpolate_default_gradient(data, c1, k);
}

void	interpolate_default_gradient(t_fdf *data, t_color c1, t_dp3 k)
{
	double	z;
	double	min_z;
	size_t	i;
	t_color	c3;

	min_z = min_z_world(data);
	i = 0;
	while (i < (size_t)(data->rows * data->cols))
	{
		z = data->world[i / data->cols][i % data->cols].z;
		c3.r = (unsigned int)(k.x * (z - min_z) + c1.r);
		c3.g = (unsigned int)(k.y * (z - min_z) + c1.g);
		c3.b = (unsigned int)(k.z * (z - min_z) + c1.b);
		c3.a = 0xff;
		c3.rgba = c3.r << 24 | c3.g << 16 | c3.b << 8 | c3.a;
		data->pixels[i / data->cols][i % data->cols].rgba = c3.rgba;
		i++;
	}
}

int	no_color_info(t_fdf *data)
{
	int	i;
	int	j;

	i = -1;
	while (++i < data->rows)
	{
		j = -1;
		while (++j < data->cols)
			if (data->pixels[i][j].rgba != WHITE)
				return (0);
	}
	return (1);
}

void	calculate_channels(t_color *c)
{
	c->r = c->rgba >> 24 & 0xff;
	c->g = c->rgba >> 16 & 0xff;
	c->b = c->rgba >> 8 & 0xff;
	c->a = c->rgba & 0xff;
}

