/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line_segment_drawing_bonus.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvarila <jvarila@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 13:25:14 by jvarila           #+#    #+#             */
/*   Updated: 2025/01/29 11:13:08 by jvarila          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/fdf.h"

static t_linvars	calculate_linvars(t_pixel p1, t_pixel p2);
static void			draw_vertical(t_fdf *data,
						t_pixel p1, t_pixel p2, t_linvars l);
static void			draw_gentle(t_fdf *data,
						t_pixel p1, t_pixel p2, t_linvars l);
static void			draw_steep(t_fdf *data,
						t_pixel p1, t_pixel p2, t_linvars l);

void	draw_segment(t_fdf *data, t_pixel p1, t_pixel p2)
{
	t_linvars		l;

	l = calculate_linvars(p1, p2);
	if (l.delta_x == 0)
		draw_vertical(data, p1, p2, l);
	else if (l.slope <= 1)
		draw_gentle(data, p1, p2, l);
	else
		draw_steep(data, p1, p2, l);
}

static void	draw_vertical(t_fdf *data, t_pixel p1, t_pixel p2, t_linvars l)
{
	int	x;
	int	y;
	int	rgba;
	int	i;

	i = -1;
	while (++i < abs(l.delta_y) + 1)
	{
		x = p1.x;
		y = (int)(p1.y + i * l.step_y);
		rgba = interpolate_color_channels(p1, p2, abs(l.delta_y), i);
		if (x >= 0 && x < (int)data->img->width
			&& y >= 0 && y < (int)data->img->height)
			mlx_put_pixel(data->img, p1.x, (int)(p1.y + i * l.step_y), rgba);
	}
}

static void	draw_gentle(t_fdf *data, t_pixel p1, t_pixel p2, t_linvars l)
{
	int	x;
	int	y;
	int	rgba;
	int	i;

	i = -1;
	while (++i < ft_iabs(l.delta_x) + 1)
	{
		x = p1.x + i * l.step_x;
		y = (int)(p1.y + i * l.step_y * l.slope);
		rgba = interpolate_color_channels(p1, p2, abs(l.delta_x), i);
		if (x >= 0 && x < (int)data->img->width
			&& y >= 0 && y < (int)data->img->height)
			mlx_put_pixel(data->img, x, y, rgba);
	}
}

static void	draw_steep(t_fdf *data, t_pixel p1, t_pixel p2, t_linvars l)
{
	int	x;
	int	y;
	int	rgba;
	int	i;

	i = -1;
	while (++i < ft_iabs(l.delta_y) + 1)
	{
		y = p1.y + i * l.step_y;
		x = (int)(p1.x + i * l.step_x * 1 / l.slope);
		rgba = interpolate_color_channels(p1, p2, abs(l.delta_y), i);
		if (x >= 0 && x < (int)data->img->width
			&& y >= 0 && y < (int)data->img->height)
			mlx_put_pixel(data->img, x, y, rgba);
	}
}

static t_linvars	calculate_linvars(t_pixel p1, t_pixel p2)
{
	t_linvars	l;

	l.delta_x = p2.x - p1.x;
	l.delta_y = p2.y - p1.y;
	if (l.delta_x)
		l.slope = fabs((double)l.delta_y / l.delta_x);
	else
		l.slope = INT_MAX;
	l.step_x = 1 + (l.delta_x < 0) * (-2);
	l.step_y = 1 + (l.delta_y < 0) * (-2);
	return (l);
}
