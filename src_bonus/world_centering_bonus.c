/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   world_centering_bonus.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvarila <jvarila@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 12:42:40 by jvarila           #+#    #+#             */
/*   Updated: 2025/01/29 11:12:55 by jvarila          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/fdf.h"

void	center_world(t_fdf *data)
{
	center_world_x(data);
	center_world_y(data);
	center_world_z(data);
}

void	center_world_x(t_fdf *data)
{
	double	min_x;
	double	max_x;
	t_dp3	offset;

	min_x = min_x_world(data);
	max_x = max_x_world(data);
	offset = (t_dp3){
		.x = - (max_x + min_x) / 2,
		.y = 0,
		.z = 0};
	offset_world(data, offset);
}

void	center_world_y(t_fdf *data)
{
	double	min_y;
	double	max_y;
	t_dp3	offset;

	min_y = min_y_world(data);
	max_y = max_y_world(data);
	offset = (t_dp3){
		.x = 0,
		.y = - (max_y + min_y) / 2,
		.z = 0};
	offset_world(data, offset);
}

void	center_world_z(t_fdf *data)
{
	double	min_z;
	double	max_z;
	t_dp3	offset;

	min_z = min_z_world(data);
	max_z = max_z_world(data);
	offset = (t_dp3){
		.x = 0,
		.y = 0,
		.z = - (max_z + min_z) / 2};
	offset_world(data, offset);
}
