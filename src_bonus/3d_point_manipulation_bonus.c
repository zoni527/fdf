/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   3d_point_manipulation_bonus.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvarila <jvarila@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 12:30:50 by jvarila           #+#    #+#             */
/*   Updated: 2025/01/29 11:13:46 by jvarila          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/fdf.h"

void	offset_dp3(t_dp3 *point, t_dp3 offset)
{
	point->x += offset.x;
	point->y += offset.y;
	point->z += offset.z;
}

void	scale_dp3(t_dp3 *point, double multiplier)
{
	point->x *= multiplier;
	point->y *= multiplier;
	point->z *= multiplier;
}

void	rotate_dp3(t_dp3 *point, double rx, double ry, double rz)
{
	double	**x_rot;
	double	**y_rot;
	double	**z_rot;

	x_rot = (double *[]){
		(double []){1, 0, 0},
		(double []){0, cos(rx), -sin(rx)},
		(double []){0, sin(rx), cos(rx)}};
	y_rot = (double *[]){
		(double []){cos(ry), 0, sin(ry)},
		(double []){0, 1, 0},
		(double []){-sin(ry), 0, cos(ry)}};
	z_rot = (double *[]){
		(double []){cos(rz), -sin(rz), 0},
		(double []){sin(rz), cos(rz), 0},
		(double []){0, 0, 1}};
	matrix_transform(point, x_rot);
	matrix_transform(point, y_rot);
	matrix_transform(point, z_rot);
}

void	matrix_transform(t_dp3 *p, double **t)
{
	double	x_o;
	double	y_o;
	double	z_o;

	x_o = p->x;
	y_o = p->y;
	z_o = p->z;
	p->x = t[0][0] * x_o + t[0][1] * y_o + t[0][2] * z_o;
	p->y = t[1][0] * x_o + t[1][1] * y_o + t[1][2] * z_o;
	p->z = t[2][0] * x_o + t[2][1] * y_o + t[2][2] * z_o;
}
