/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   view_manipulation_bonus.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvarila <jvarila@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 12:08:31 by jvarila           #+#    #+#             */
/*   Updated: 2025/01/29 12:08:39 by jvarila          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/fdf.h"

void	offset_view(t_fdf *data, t_dp2 offset)
{
	int		i;
	int		j;

	i = -1;
	while (++i < data->rows)
	{
		j = -1;
		while (++j < data->cols)
			offset_dp2(&data->view[i][j], offset);
	}
}

void	scale_view(t_fdf *data, double multiplier)
{
	int		i;
	int		j;

	i = -1;
	while (++i < data->rows)
	{
		j = -1;
		while (++j < data->cols)
			scale_dp2(&data->view[i][j], multiplier);
	}
}

void	rotate_view(t_fdf *data, double angle)
{
	int		i;
	int		j;

	i = -1;
	while (++i < data->rows)
	{
		j = -1;
		while (++j < data->cols)
			rotate_dp2(&data->view[i][j], angle);
	}
}

void	stretch_view_to_window_scale(t_fdf *data)
{
	double	width_x;
	double	width_y;
	double	ratio_x;
	double	ratio_y;

	ratio_x = INFINITY;
	ratio_y = INFINITY;
	data->view_scale = 1;
	width_x = max_x_view(data) - min_x_view(data);
	width_y = max_y_view(data) - min_y_view(data);
	if (width_y == 0 && width_x == 0)
		return ;
	if (width_x != 0)
		ratio_x = WIDTH / width_x;
	if (width_y != 0)
		ratio_y = WIDTH / width_y;
	if (ratio_x == INFINITY)
		data->view_scale = ratio_y;
	if (ratio_y == INFINITY)
		data->view_scale = ratio_x;
	if (ratio_y < ratio_x)
		data->view_scale = ratio_y;
	else
		data->view_scale = ratio_x;
	scale_view(data, data->view_scale);
}
