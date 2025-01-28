/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_02.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvarila <jvarila@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 16:53:26 by jvarila           #+#    #+#             */
/*   Updated: 2025/01/27 16:56:57 by jvarila          ###   ########.fr       */
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
