/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvarila <jvarila@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 14:04:17 by jvarila           #+#    #+#             */
/*   Updated: 2025/01/23 14:39:26 by jvarila          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/fdf.h"

void	hook(void *param)
{
	t_fdf	*data;

	data = (t_fdf *)param;
	if (mlx_is_key_down(data->mlx, MLX_KEY_ESCAPE))
		mlx_close_window(data->mlx);
	if (mlx_is_key_down(data->mlx, MLX_KEY_UP))
	{
		data->view_offset.y -= 10;
		offset_view(data, (t_dp2){.x = 0, .y = -10});
	}
	if (mlx_is_key_down(data->mlx, MLX_KEY_DOWN))
	{
		data->view_offset.y += 10;
		offset_view(data, (t_dp2){.x = 0, .y = 10});
	}
	if (mlx_is_key_down(data->mlx, MLX_KEY_RIGHT))
	{
		data->view_offset.x += 10;
		offset_view(data, (t_dp2){.x = 10, .y = 0});
	}
	if (mlx_is_key_down(data->mlx, MLX_KEY_LEFT))
	{
		data->view_offset.x -= 10;
		offset_view(data, (t_dp2){.x = -10, .y = 0});
	}
	if (mlx_is_key_down(data->mlx, MLX_KEY_S))
	{
		rotate_world(data, 5 * ONE_DEGREE, 0, 0);
		y_plane_projection(data);
		scale_view(data, data->view_scale);
		rotate_view(data, M_PI);
		offset_view(data, data->view_offset);
	}
	if (mlx_is_key_down(data->mlx, MLX_KEY_W))
	{
		rotate_world(data, -5 * ONE_DEGREE, 0, 0);
		y_plane_projection(data);
		scale_view(data, data->view_scale);
		rotate_view(data, M_PI);
		offset_view(data, data->view_offset);
	}
	if (mlx_is_key_down(data->mlx, MLX_KEY_A))
	{
		rotate_world(data, 0, 0, -5 * ONE_DEGREE);
		y_plane_projection(data);
		scale_view(data, data->view_scale);
		rotate_view(data, M_PI);
		offset_view(data, data->view_offset);
	}
	if (mlx_is_key_down(data->mlx, MLX_KEY_D))
	{
		rotate_world(data, 0, 0, 5 * ONE_DEGREE);
		y_plane_projection(data);
		scale_view(data, data->view_scale);
		rotate_view(data, M_PI);
		offset_view(data, data->view_offset);
	}
	if (mlx_is_key_down(data->mlx, MLX_KEY_Q))
	{
		rotate_world(data, 0, -5 * ONE_DEGREE, 0);
		y_plane_projection(data);
		scale_view(data, data->view_scale);
		rotate_view(data, M_PI);
		offset_view(data, data->view_offset);
	}
	if (mlx_is_key_down(data->mlx, MLX_KEY_E))
	{
		rotate_world(data, 0, 5 * ONE_DEGREE, 0);
		y_plane_projection(data);
		scale_view(data, data->view_scale);
		rotate_view(data, M_PI);
		offset_view(data, data->view_offset);
	}
	redraw(data);
}

void	scroll_hook(double xdelta, double ydelta, void *param)
{
	t_fdf	*data;
	t_dp2	offset_o;
	t_dp2	offset_temp;

	(void)xdelta;
	data = param;
	offset_o = data->view_offset;
	if (ydelta > 0)
	{
		center_view(data);
		data->view_scale *= 1.1;
		scale_view(data, 1.1);
		offset_temp.x = -data->view_offset.x;
		offset_temp.y = -data->view_offset.y;
		offset_view(data, offset_temp);
		data->view_offset = offset_o;
	}
	if (ydelta < 0)
	{
		center_view(data);
		data->view_scale *= 0.9;
		scale_view(data, 0.9);
		offset_temp.x = -data->view_offset.x;
		offset_temp.y = -data->view_offset.y;
		offset_view(data, offset_temp);
		data->view_offset = offset_o;
	}
	redraw(data);
}
