/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   view_min_max_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvarila <jvarila@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 12:38:57 by jvarila           #+#    #+#             */
/*   Updated: 2025/01/29 11:12:48 by jvarila          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/fdf.h"

double	min_x_view(t_fdf *data)
{
	double	min_x;
	int		i;
	int		j;

	min_x = data->view[0][0].x;
	i = -1;
	while (++i < data->rows)
	{
		j = -1;
		while (++j < data->cols)
		{
			if (data->view[i][j].x < min_x)
				min_x = data->view[i][j].x;
		}
	}
	return (min_x);
}

double	min_y_view(t_fdf *data)
{
	double	min_y;
	int		i;
	int		j;

	min_y = data->view[0][0].y;
	i = -1;
	while (++i < data->rows)
	{
		j = -1;
		while (++j < data->cols)
		{
			if (data->view[i][j].y < min_y)
				min_y = data->view[i][j].y;
		}
	}
	return (min_y);
}

double	max_x_view(t_fdf *data)
{
	double	max_x;
	int		i;
	int		j;

	max_x = data->view[0][0].x;
	i = -1;
	while (++i < data->rows)
	{
		j = -1;
		while (++j < data->cols)
		{
			if (data->view[i][j].x > max_x)
				max_x = data->view[i][j].x;
		}
	}
	return (max_x);
}

double	max_y_view(t_fdf *data)
{
	double	max_y;
	int		i;
	int		j;

	max_y = data->view[0][0].y;
	i = -1;
	while (++i < data->rows)
	{
		j = -1;
		while (++j < data->cols)
		{
			if (data->view[i][j].y > max_y)
				max_y = data->view[i][j].y;
		}
	}
	return (max_y);
}
