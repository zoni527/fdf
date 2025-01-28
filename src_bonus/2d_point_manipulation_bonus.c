/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   2d_point_manipulation.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvarila <jvarila@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 11:31:59 by jvarila           #+#    #+#             */
/*   Updated: 2025/01/23 11:40:44 by jvarila          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/fdf.h"

void	offset_dp2(t_dp2 *point, t_dp2 offset)
{
	point->x += offset.x;
	point->y += offset.y;
}

void	scale_dp2(t_dp2 *point, double multiplier)
{
	point->x *= multiplier;
	point->y *= multiplier;
}

void	rotate_dp2(t_dp2 *point, double angle)
{
	double	x_o;
	double	y_o;

	x_o = point->x;
	y_o = point->y;
	point->x = x_o * cos(angle) - y_o * sin(angle);
	point->y = x_o * sin(angle) + y_o * cos(angle);
}
