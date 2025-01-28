/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pixels_and_colors.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvarila <jvarila@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 13:16:37 by jvarila           #+#    #+#             */
/*   Updated: 2025/01/23 13:25:05 by jvarila          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/fdf.h"

void	initialize_pixels(t_fdf *data)
{
	t_dp2	offset;

	stretch_view_to_window_scale(data);
	scale_view(data, 0.9);
	data->view_scale *= 0.9;
	rotate_view(data, M_PI);
	center_view(data);
	offset.x = (double)WIDTH / 2;
	offset.y = (double)HEIGHT / 2;
	data->view_offset.x += offset.x;
	data->view_offset.y += offset.y;
	offset_view(data, offset);
	view_to_pixels(data);
}

void	view_to_pixels(t_fdf *data)
{
	int	i;
	int	j;

	i = -1;
	while (++i < data->rows)
	{
		j = -1;
		while (++j < data->cols)
		{
			data->pixels[i][j].x = (int)data->view[i][j].x;
			data->pixels[i][j].y = (int)data->view[i][j].y;
		}
	}
}

int	interpolate_color_channels(t_pixel p1, t_pixel p2, double d, int i)
{
	int	r;
	int	g;
	int	b;
	int	rgba;

	r = interpolate_channel(p1.rgba >> 24 & 0xff, p2.rgba >> 24 & 0xff, d, i);
	g = interpolate_channel(p1.rgba >> 16 & 0xff, p2.rgba >> 16 & 0xff, d, i);
	b = interpolate_channel(p1.rgba >> 8 & 0xff, p2.rgba >> 8 & 0xff, d, i);
	rgba = (r << 24 | g << 16 | b << 8 | 0xff);
	return (rgba);
}

int	interpolate_channel(int p1_c, int p2_c, double distance, int index)
{
	return ((int)(p1_c + index * (double)(p2_c - p1_c) / distance));
}
