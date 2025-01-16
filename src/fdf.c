/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvarila <jvarila@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 09:42:50 by jvarila           #+#    #+#             */
/*   Updated: 2025/01/13 09:45:21 by jvarila          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/fdf.h"
#include <fcntl.h>

void		draw_segment(mlx_image_t *img, t_ip2 p1, t_ip2 p2, int rgba);
void		fill_with_color(mlx_image_t *img, int rgba);
void		draw_test_reticle(mlx_image_t *img, int rgba);
t_linvars	calculate_linvars(t_ip2 p1, t_ip2 p2);

int			create_points(t_dp3arr *points_array, const t_matrix *matrix);
void		assign_points(t_dp3arr *points_array, const t_matrix *matrix);
void		free_points(t_dp3arr *points_array);

void		matrix_transform(t_dp3 *p, t_tmatrix *t);

void		offset_point(t_dp3 *point, t_dp3 offset);
void		rotate_point(t_dp3 *point, double rx, double ry, double rz);
void		scale_point(t_dp3 *point, double multiplier);

void		offset_points(t_dp3arr *points_array, t_dp3 offset);
void		scale_points(t_dp3arr *points_array, double multiplier);
void		rotate_points(t_dp3arr *points_array, double rx, double ry, double rz);

void		center_points(t_dp3arr *points_array);

int	main(int argc, char *argv[])
{
	mlx_t		*mlx;
	mlx_image_t	*img;
	t_matrix	map;
	t_dp3arr		points;

	if (argc != 2)
		return (write_error_return_int("Usage: ./fdf <file_name>", 1));
	ft_memset(&map, 0, sizeof(map));
	if (validate_map_file(argv[1]))
		return (write_error_return_int("ERROR: invalid map file, "
				"check the file name and/or formatting", 1));
	if (parse_map_file(argv[1], &map) == FAILURE)
		return (write_error_return_int("ERROR: failed to parse file", 1));
	print_matrix(&map);
	if (create_points(&points, &map) == FAILURE)
	{
		free_matrix_elements(&map);
		return (write_error_return_int("ERROR: failed to create 3D points", 1));
	}
	center_points(&points);
	mlx = mlx_init(WIDTH, HEIGHT, "fdf", 1);
	if (!mlx)
	{
		free_matrix_elements(&map);
		free_points(&points);
		return (write_error_return_int("ERROR: couldn't init mlx", 1));
	}
	img = mlx_new_image(mlx, WIDTH, HEIGHT);
	if (!img || mlx_image_to_window(mlx, img, 0, 0) < 0)
		return (free_ptr_return_int((void *)&mlx, 1));
	fill_with_color(img, RED | GREEN);
	draw_test_reticle(img, BLACK);
	mlx_loop(mlx);
	mlx_terminate(mlx);
	free_matrix_elements(&map);
	free_points(&points);
	return (0);
}

void	draw_segment(mlx_image_t *img, t_ip2 p1, t_ip2 p2, int rgba)
{
	t_linvars	l;
	int			i;
	int			x;
	int			y;

	l = calculate_linvars(p1, p2);
	i = -1;
	if (l.delta_x == 0)
	{
		while (++i < abs(l.delta_y) + 1)
			mlx_put_pixel(img, p1.x, (int)(p1.y + i * l.step_y), rgba);
		return ;
	}
	while (++i < l.delta_x + 1)
	{
		x = p1.x + i * l.step_x;
		y = (int)(p1.y + i * l.step_y * l.slope);
		if (x >= 0 && x < (int)img->width && y >= 0 && y < (int)img->height)
			mlx_put_pixel(img, x, y, rgba);
	}
}

t_linvars	calculate_linvars(t_ip2 p1, t_ip2 p2)
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

void	draw_test_reticle(mlx_image_t *img, int rgba)
{
	t_ip2	p1;
	t_ip2	p2;

	p1 = (t_ip2){.x = 0, .y = 0};
	p2 = (t_ip2){.x = 0, .y = 0};
	draw_segment(img, p1, p2, rgba);
	p1 = (t_ip2){.x = WIDTH - 1, .y = HEIGHT - 1};
	p2 = (t_ip2){.x = 0, .y = 0};
	draw_segment(img, p1, p2, rgba);
	p1 = (t_ip2){.x = 0, .y = 0};
	p2 = (t_ip2){.x = WIDTH - 1, .y = HEIGHT - 1};
	draw_segment(img, p1, p2, rgba);
	p1 = (t_ip2){.x = 0, .y = HEIGHT - 1};
	p2 = (t_ip2){.x = WIDTH - 1, .y = 0};
	draw_segment(img, p1, p2, rgba);
	p1 = (t_ip2){.x = WIDTH - 1, .y = 0};
	p2 = (t_ip2){.x = 0, .y = HEIGHT - 1};
	draw_segment(img, p1, p2, rgba);
	p1 = (t_ip2){.x = WIDTH / 2, .y = 0};
	p2 = (t_ip2){.x = WIDTH / 2, .y = HEIGHT - 1};
	draw_segment(img, p1, p2, rgba);
	p1 = (t_ip2){.x = WIDTH / 2, .y = HEIGHT - 1};
	p2 = (t_ip2){.x = WIDTH / 2, .y = 0};
	draw_segment(img, p1, p2, rgba);
	p1 = (t_ip2){.x = 0, .y = HEIGHT / 2};
	p2 = (t_ip2){.x = WIDTH - 1, .y = HEIGHT / 2};
	draw_segment(img, p1, p2, rgba);
	p1 = (t_ip2){.x = WIDTH - 1, .y = HEIGHT / 2};
	p2 = (t_ip2){.x = 0, .y = HEIGHT / 2};
	draw_segment(img, p1, p2, rgba);
	p1 = (t_ip2){.x = WIDTH / 2, .y = 0};
	p2 = (t_ip2){.x = 0, .y = HEIGHT / 2};
	draw_segment(img, p1, p2, rgba);
	p1 = (t_ip2){.x = 0, .y = HEIGHT / 2};
	p2 = (t_ip2){.x = WIDTH / 2, .y = 0};
	draw_segment(img, p1, p2, rgba);
	p1 = (t_ip2){.x = WIDTH - 1, .y = HEIGHT / 2};
	p2 = (t_ip2){.x = WIDTH / 2, .y = 0};
	draw_segment(img, p1, p2, rgba);
	p1 = (t_ip2){.x = WIDTH / 2, .y = 0};
	p2 = (t_ip2){.x = WIDTH - 1, .y = HEIGHT / 2};
	draw_segment(img, p1, p2, rgba);
	p1 = (t_ip2){.x = WIDTH / 2, .y = HEIGHT - 1};
	p2 = (t_ip2){.x = 0, .y = HEIGHT / 2};
	draw_segment(img, p1, p2, rgba);
	p1 = (t_ip2){.x = 0, .y = HEIGHT / 2};
	p2 = (t_ip2){.x = WIDTH / 2, .y = HEIGHT - 1};
	draw_segment(img, p1, p2, rgba);
	p1 = (t_ip2){.x = WIDTH / 2, .y = HEIGHT - 1};
	p2 = (t_ip2){.x = WIDTH - 1, .y = HEIGHT / 2};
	draw_segment(img, p1, p2, rgba);
	p1 = (t_ip2){.x = WIDTH - 1, .y = HEIGHT / 2};
	p2 = (t_ip2){.x = WIDTH / 2, .y = HEIGHT - 1};
	draw_segment(img, p1, p2, rgba);
}

void	fill_with_color(mlx_image_t *img, int rgba)
{
	int		w;
	int		h;
	size_t	i;

	w = img->width;
	h = img->height;
	i = 0;
	while (i < (size_t)(w * h))
	{
		mlx_put_pixel(img, i % w, (i / w) % h, rgba);
		i++;
	}
}

int	create_points(t_dp3arr *points_array, const t_matrix *matrix)
{
	int	i;

	points_array->rows = matrix->rows;
	points_array->cols = matrix->cols;
	points_array->points = ft_calloc(points_array->rows, sizeof(t_dp3 *));
	if (!points_array->points)
		return (FAILURE);
	i = -1;
	while (++i < points_array->rows)
	{
		points_array->points[i] = malloc(points_array->cols * sizeof(t_dp3));
		if (!points_array->points[i])
		{
			free_points(points_array);
			return (FAILURE);
		}
	}
	assign_points(points_array, matrix);
	return (SUCCESS);
}

void	assign_points(t_dp3arr *points_array, const t_matrix *matrix)
{
	int	i;
	int	j;

	i = -1;
	while (++i < points_array->rows)
	{
		j = -1;
		while (++j < points_array->cols)
		{
			points_array->points[i][j].x = i;
			points_array->points[i][j].y = j;
			points_array->points[i][j].z = matrix->elem[i][j];
		}
	}
}

void	free_points(t_dp3arr *points_array)
{
	if (points_array->rows)
		while (points_array->rows--)
			free(points_array->points[points_array->rows]);
	free(points_array->points);
	points_array->points = NULL;
	points_array->cols = 0;
	points_array->rows = 0;
}

void	offset_point(t_dp3 *point, t_dp3 offset)
{
	point->x += offset.x;
	point->y += offset.y;
	point->z += offset.z;
}

void	scale_point(t_dp3 *point, double multiplier)
{
	point->x *= multiplier;
	point->y *= multiplier;
	point->z *= multiplier;
}

void	matrix_transform(t_dp3 *p, t_tmatrix *t)
{
	p->x = t->elem[0][0] * p->x + t->elem[0][1] * p->y + t->elem[0][2] * p->z;
	p->y = t->elem[1][0] * p->x + t->elem[1][1] * p->y + t->elem[1][2] * p->z;
	p->z = t->elem[2][0] * p->x + t->elem[2][1] * p->y + t->elem[2][2] * p->z;
}

void	rotate_point(t_dp3 *point, double rx, double ry, double rz)
{
	t_tmatrix	x_rot;
	t_tmatrix	y_rot;
	t_tmatrix	z_rot;

	x_rot.elem = (double **)(double [3][3]){
	{1, 0, 0},
	{0, cos(rx), -sin(rx)},
	{0, sin(rx), cos(rx)}};
	y_rot.elem = (double **)(double [3][3]){
	{cos(ry), 0, sin(ry)},
	{0, 1, 0},
	{-sin(ry), 0, cos(ry)}};
	z_rot.elem = (double **)(double [3][3]){
	{cos(rz), -sin(rz), 0},
	{sin(rz), cos(rz), 0},
	{0, 0, 1}};
	matrix_transform(point, &x_rot);
	matrix_transform(point, &y_rot);
	matrix_transform(point, &z_rot);
}

void	rotate_points(t_dp3arr *points_array, double rx, double ry, double rz)
{
	int	i;
	int	j;

	i = -1;
	while (++i < points_array->rows)
	{
		j = -1;
		while (++j < points_array->cols)
			rotate_point(&points_array->points[i][j], rx, ry, rz);
	}
}

void	scale_points(t_dp3arr *points_array, double multiplier)
{
	int	i;
	int	j;

	i = -1;
	while (++i < points_array->rows)
	{
		j = -1;
		while (++j < points_array->cols)
			scale_point(&points_array->points[i][j], multiplier);
	}
}

void	offset_points(t_dp3arr *points_array, t_dp3 offset)
{
	int	i;
	int	j;

	i = -1;
	while (++i < points_array->rows)
	{
		j = -1;
		while (++j < points_array->cols)
			offset_point(&points_array->points[i][j], offset);
	}
}

void	center_points(t_dp3arr *arr)
{
	double	min_x;
	double	max_x;
	double	min_y;
	double	max_y;
	double	min_z;
	double	max_z;
	t_dp3	offset;
	size_t	i;

	offset = arr->points[0][0];
	scale_point(&offset, -1);
	offset_points(arr, offset);
	min_x = arr->points[0][0].x;
	max_x = arr->points[0][0].x;
	min_y = arr->points[0][0].y;
	max_y = arr->points[0][0].y;
	min_z = arr->points[0][0].z;
	max_z = arr->points[0][0].z;
	i = 1;
	while (i < (size_t)(arr->cols * arr->rows))
	{
		if (arr->points[i / arr->rows][i % arr->cols].x < min_x)
			min_x = arr->points[i / arr->rows][i % arr->cols].x;
		if (arr->points[i / arr->rows][i % arr->cols].x > max_x)
			max_x = arr->points[i / arr->rows][i % arr->cols].x;
		if (arr->points[i / arr->rows][i % arr->cols].y < min_y)
			min_y = arr->points[i / arr->rows][i % arr->cols].y;
		if (arr->points[i / arr->rows][i % arr->cols].y > max_y)
			max_y = arr->points[i / arr->rows][i % arr->cols].y;
		if (arr->points[i / arr->rows][i % arr->cols].z < min_z)
			min_z = arr->points[i / arr->rows][i % arr->cols].z;
		if (arr->points[i / arr->rows][i % arr->cols].z > max_z)
			max_z = arr->points[i / arr->rows][i % arr->cols].z;
		i++;
	}
	offset = (t_dp3){
		.x = - (max_x + min_x) / 2,
		.y = - (max_y + min_y) / 2,
		.z = - (max_z + min_z) / 2};
	offset_points(arr, offset);
}

void	y_plane_projection(t_dp2arr *view, t_dp3arr *world)
{
	int	i;
	int	j;

	if (view->rows != world->rows || view->cols != world->cols)
		return ;
	i = -1;
	while (++i < view->rows)
	{
		j = -1;
		while (++j < view->cols)
		{
			view->points[i][j].x = world->points[i][j].x;
			view->points[i][j].y = world->points[i][j].z;
		}
	}
}

void	rotate_view(t_dp2arr *view, double rot)
{
	int		i;
	int		j;
	double	x;
	double	y;

	i = -1;
	while (++i < view->rows)
	{
		j = -1;
		while (++j < view->cols)
		{
			x = view->points[i][j].x;
			y = view->points[i][j].y;
			view->points[i][j].x = x * cos(rot) - y * sin(rot);
			view->points[i][j].y = x * sin(rot) + y * cos(rot);
		}
	}
}

void	scale_view(t_dp2arr *view, double multiplier)
{

	int		i;
	int		j;

	i = -1;
	while (++i < view->rows)
	{
		j = -1;
		while (++j < view->cols)
		{
			view->points[i][j].x *= multiplier;
			view->points[i][j].y *= multiplier;
		}
	}
}

void	offset_view(t_dp2arr *view, t_dp2 *offset)
{
	int		i;
	int		j;

	i = -1;
	while (++i < view->rows)
	{
		j = -1;
		while (++j < view->cols)
		{
			view->points[i][j].x += offset->x;
			view->points[i][j].y += offset->y;
		}
	}
}

void	view_to_image(t_ip2arr *pixels, const t_dp2arr *view,
				   int window_width, int window_height)
{
	int	offset_x;
	int	offset_y;

	offset_x = window_width / 2;
	offset_y = window_height / 2;
	
}
