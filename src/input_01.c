/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_01.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvarila <jvarila@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 14:04:17 by jvarila           #+#    #+#             */
/*   Updated: 2025/01/27 16:40:34 by jvarila          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/fdf.h"

static void	rotate(t_fdf *data, double rx, double ry, double rz);
static void	translate(t_fdf *data, double offset_x, double offset_y);

void	hook(void *param)
{
	t_fdf	*data;

	data = (t_fdf *)param;
	if (mlx_is_key_down(data->mlx, MLX_KEY_UP))
		translate(data, 0, -20);
	if (mlx_is_key_down(data->mlx, MLX_KEY_DOWN))
		translate(data, 0, 20);
	if (mlx_is_key_down(data->mlx, MLX_KEY_RIGHT))
		translate(data, 20, 0);
	if (mlx_is_key_down(data->mlx, MLX_KEY_LEFT))
		translate(data, -20, 0);
	if (mlx_is_key_down(data->mlx, MLX_KEY_S))
		rotate(data, 5 * ONE_DEGREE, 0, 0);
	if (mlx_is_key_down(data->mlx, MLX_KEY_W))
		rotate(data, -5 * ONE_DEGREE, 0, 0);
	if (mlx_is_key_down(data->mlx, MLX_KEY_A))
		rotate(data, 0, 0, 5 * ONE_DEGREE);
	if (mlx_is_key_down(data->mlx, MLX_KEY_D))
		rotate(data, 0, 0, -5 * ONE_DEGREE);
	if (mlx_is_key_down(data->mlx, MLX_KEY_Q))
		rotate(data, 0, 5 * ONE_DEGREE, 0);
	if (mlx_is_key_down(data->mlx, MLX_KEY_E))
		rotate(data, 0, -5 * ONE_DEGREE, 0);
	redraw(data);
}

void	esc_hook(void *param)
{
	t_fdf	*data;

	data = (t_fdf *)param;
	if (mlx_is_key_down(data->mlx, MLX_KEY_ESCAPE))
		mlx_close_window(data->mlx);
}

static void	translate(t_fdf *data, double offset_x, double offset_y)
{
	t_dp2	offset;

	data->view_offset.x += offset_x;
	data->view_offset.y += offset_y;
	offset.x = offset_x;
	offset.y = offset_y;
	offset_view(data, offset);
}

static void	rotate(t_fdf *data, double rx, double ry, double rz)
{
	rotate_world(data, rx, ry, rz);
	data->project(data);
	scale_view(data, data->view_scale);
	rotate_view(data, M_PI);
	offset_view(data, data->view_offset);
}
