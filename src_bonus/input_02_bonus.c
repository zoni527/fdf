/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_02.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvarila <jvarila@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 16:53:26 by jvarila           #+#    #+#             */
/*   Updated: 2025/01/28 14:15:28 by jvarila          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/fdf.h"

static void	scale(t_fdf *data, double multiplier);

void	scroll_hook(double xdelta, double ydelta, void *param)
{
	t_fdf	*data;

	(void)xdelta;
	data = param;
	if (ydelta > 0)
		scale(data, 1.1);
	if (ydelta < 0)
		scale(data, 0.9);
}

static void	scale(t_fdf *data, double multiplier)
{
	t_dp2	offset_o;
	t_dp2	offset_temp;

	offset_o = data->view_offset;
	center_view(data);
	data->view_scale *= multiplier;
	scale_view(data, multiplier);
	offset_temp.x = -data->view_offset.x;
	offset_temp.y = -data->view_offset.y;
	offset_view(data, offset_temp);
	data->view_offset = offset_o;
}

void	projection_hook(void *param)
{
	t_fdf	*data;

	data = (t_fdf *)param;
	if (mlx_is_key_down(data->mlx, MLX_KEY_X))
		data->project = &x_plane_projection;
	if (mlx_is_key_down(data->mlx, MLX_KEY_Y))
		data->project = &y_plane_projection;
	if (mlx_is_key_down(data->mlx, MLX_KEY_Z))
		data->project = &z_plane_projection;
	if (mlx_is_key_down(data->mlx, MLX_KEY_O))
		data->project = &oblique_projection;
}

void	scale_z_hook(void *param)
{
	t_fdf	*data;

	data = (t_fdf *)param;
	if (mlx_is_key_down(data->mlx, MLX_KEY_EQUAL))
	{
		scale_world_z(data, 1.1);
		data->project(data);
		scale_view(data, data->view_scale);
		rotate_view(data, M_PI);
		offset_view(data, data->view_offset);
	}
	if (mlx_is_key_down(data->mlx, MLX_KEY_MINUS))
	{
		scale_world_z(data, 0.9);
		data->project(data);
		scale_view(data, data->view_scale);
		rotate_view(data, M_PI);
		offset_view(data, data->view_offset);
	}
}

void	reset_hook(void *param)
{
	t_fdf	*data;

	data = (t_fdf *)param;
	if (mlx_is_key_down(data->mlx, MLX_KEY_R))
	{
		data->view_offset.x = 0;
		data->view_offset.y = 0;
		assign_world(data);
		center_world(data);
		data->project(data);
		initialize_pixels(data);
	}
}
