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

void		draw_segment(mlx_image_t *img, t_ip2 p1, t_ip2 p2,
				   unsigned int rgba);
void		fill_with_color(mlx_image_t *img, unsigned int rgba);
void		draw_test_reticle(mlx_image_t *img, int rgba);
t_linvars	calculate_linvars(t_ip2 p1, t_ip2 p2);

int			create_points(t_dp3arr *points_array, const t_imatrix *matrix);
void		assign_points(t_dp3arr *points_array, const t_imatrix *matrix);
void		free_points(t_dp3arr *points_array);

void		matrix_transform(t_dp3 *p, t_dmatrix *t);

void		offset_point(t_dp3 *point, t_dp3 offset);
void		rotate_point(t_dp3 *point, double rx, double ry, double rz);
void		scale_point(t_dp3 *point, double multiplier);

void		offset_points(t_dp3arr *points_array, t_dp3 offset);
void		scale_points(t_dp3arr *points_array, double multiplier);
void		rotate_points(t_dp3arr *points_array, double rx, double ry, double rz);

void		center_points(t_dp3arr *points_array);

int			create_view(t_dp2arr *view, t_dp3arr *world);
void		y_plane_projection(t_dp2arr *view, t_dp3arr *world);
void		free_view(t_dp2arr *view);

void		rotate_view(t_dp2arr *view, double rot);
void		scale_view(t_dp2arr *view, double multiplier);
void		offset_view(t_dp2arr *view, t_dp2 *offset);
void		view_to_image(t_ip2arr *pixels, t_dp2arr *view,
					   int window_width, int window_height);

int			create_pixels(t_ip2arr *pixels, const t_imatrix *map);
void		draw_map(t_ip2arr *pix, t_imatrix *map, mlx_image_t *img);
void		free_pixels(t_ip2arr *pixels);

void		free_data(t_fdf_data *data);
void		free_print_exit(t_fdf_data *data, const char *error);

int	main(int argc, char *argv[])
{
	t_fdf_data	data;

	if (argc != 2)
		return (write_error_return_int("Usage: ./fdf <file_name>", 1));
	ft_memset(&data, 0, sizeof(t_fdf_data));
	if (validate_map_file(argv[1]))
		return (write_error_return_int("ERROR: invalid map file, "
				"check the file name and/or formatting", 1));
	if (parse_map_file(argv[1], &data.map) == FAILURE)
		return (write_error_return_int("ERROR: failed to parse file", 1));
	print_matrix(&data.map);
	if (create_points(&data.world, &data.map) == FAILURE)
		free_print_exit(&data, "ERROR: failed to allocate world");
	center_points(&data.world);
	rotate_points(&data.world, M_PI / 6, M_PI / 6, M_PI / 6);
	scale_points(&data.world, 0.5);
	if (create_view(&data.view, &data.world) == FAILURE)
		free_print_exit(&data, "ERROR: failed to allocate view");
	y_plane_projection(&data.view, &data.world);
	if (create_pixels(&data.pixels, &data.map) == FAILURE)
		free_print_exit(&data, "ERROR: failed to allocate pixels");
	view_to_image(&data.pixels, &data.view, WIDTH, HEIGHT);
	data.mlx = mlx_init(WIDTH, HEIGHT, "fdf", 1);
	if (!data.mlx)
		free_print_exit(&data, "ERROR: couldn't initiate mlx");
	data.img = mlx_new_image(data.mlx, WIDTH, HEIGHT);
	if (!data.img)
		free_print_exit(&data, "ERROR: couldn't create image");
	if (mlx_image_to_window(data.mlx, data.img, 0, 0) < 0)
		free_print_exit(&data, "ERROR: couldn't draw image");
	fill_with_color(data.img, BLACK);
	draw_map(&data.pixels, &data.map, data.img);
	mlx_loop(data.mlx);
	mlx_terminate(data.mlx);
	free_data(&data);
	return (0);
}

void	free_print_exit(t_fdf_data *data, const char *error)
{
	free_data(data);
	ft_memset(data, 0, sizeof(t_fdf_data));
	ft_putstr_fd(error, STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
	exit(EXIT_FAILURE);
}

void	free_data(t_fdf_data *data)
{
	free_matrix_elements(&data->map);
	free_points(&data->world);
	free_view(&data->view);
	free_pixels(&data->pixels);
}

void	draw_segment(mlx_image_t *img, t_ip2 p1, t_ip2 p2,
				  unsigned int rgba)
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
	if (l.slope <= 1)
	{
		while (++i < ft_iabs(l.delta_x) + 1)
		{
			x = p1.x + i * l.step_x;
			y = (int)(p1.y + i * l.step_y * l.slope);
			if (x >= 0 && x < (int)img->width && y >= 0 && y < (int)img->height)
				mlx_put_pixel(img, x, y, rgba);
		}
	}
	else
	{
		while (++i < ft_iabs(l.delta_y) + 1)
		{
			y = p1.y + i * l.step_y;
			x = (int)(p1.x + i * l.step_x * 1 / l.slope);
			if (x >= 0 && x < (int)img->width && y >= 0 && y < (int)img->height)
				mlx_put_pixel(img, x, y, rgba);
		}
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

void	fill_with_color(mlx_image_t *img, unsigned int rgba)
{
	int		w;
	int		h;
	size_t	i;

	w = img->width;
	h = img->height;
	i = 0;
	while (i < (size_t)(w * h))
	{
		mlx_put_pixel(img, i % w, i / w, rgba);
		i++;
	}
}

int	create_points(t_dp3arr *points_array, const t_imatrix *matrix)
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

void	assign_points(t_dp3arr *points_array, const t_imatrix *matrix)
{
	int	i;
	int	j;

	i = -1;
	while (++i < points_array->rows)
	{
		j = -1;
		while (++j < points_array->cols)
		{
			points_array->points[i][j].x = j;
			points_array->points[i][j].y = i;
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

void	matrix_transform(t_dp3 *p, t_dmatrix *t)
{
	p->x = t->elem[0][0] * p->x + t->elem[0][1] * p->y + t->elem[0][2] * p->z;
	p->y = t->elem[1][0] * p->x + t->elem[1][1] * p->y + t->elem[1][2] * p->z;
	p->z = t->elem[2][0] * p->x + t->elem[2][1] * p->y + t->elem[2][2] * p->z;
}

void	rotate_point(t_dp3 *point, double rx, double ry, double rz)
{
	t_dmatrix	x_rot;
	t_dmatrix	y_rot;
	t_dmatrix	z_rot;

	x_rot.elem = (double *[]){
	(double []){1, 0, 0},
	(double []){0, cos(rx), -sin(rx)},
	(double []){0, sin(rx), cos(rx)}
	};
	y_rot.elem = (double *[]){
	(double []){cos(ry), 0, sin(ry)},
	(double []){0, 1, 0},
	(double []){-sin(ry), 0, cos(ry)}};
	z_rot.elem = (double *[]){
	(double []){cos(rz), -sin(rz), 0},
	(double []){sin(rz), cos(rz), 0},
	(double []){0, 0, 1}};
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
		if (arr->points[i / arr->cols][i % arr->cols].x < min_x)
			min_x = arr->points[i / arr->cols][i % arr->cols].x;
		if (arr->points[i / arr->cols][i % arr->cols].x > max_x)
			max_x = arr->points[i / arr->cols][i % arr->cols].x;
		if (arr->points[i / arr->cols][i % arr->cols].y < min_y)
			min_y = arr->points[i / arr->cols][i % arr->cols].y;
		if (arr->points[i / arr->cols][i % arr->cols].y > max_y)
			max_y = arr->points[i / arr->cols][i % arr->cols].y;
		if (arr->points[i / arr->cols][i % arr->cols].z < min_z)
			min_z = arr->points[i / arr->cols][i % arr->cols].z;
		if (arr->points[i / arr->cols][i % arr->cols].z > max_z)
			max_z = arr->points[i / arr->cols][i % arr->cols].z;
		i++;
	}
	offset = (t_dp3){
		.x = - (max_x + min_x) / 2,
		.y = - (max_y + min_y) / 2,
		.z = - (max_z + min_z) / 2};
	offset_points(arr, offset);
}

int		create_view(t_dp2arr *view, t_dp3arr *world)
{
	int	i;

	view->rows = world->rows;
	view->cols = world->cols;
	view->points = ft_calloc(world->rows, sizeof(t_dp2 *));
	if (!view->points)
		return (FAILURE);
	i = -1;
	while (++i < world->rows)
	{
		view->points[i] = ft_calloc(world->cols, sizeof(t_dp2));
		if (!view->points[i])
		{
			free_view(view);
			return (FAILURE);
		}
	}
	return (SUCCESS);

}

void	free_view(t_dp2arr *view)
{
	if (view->rows)
		while (view->rows--)
			free(view->points[view->rows]);
	free(view->points);
	view->points = NULL;
	view->cols = 0;
	view->rows = 0;
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

void	view_to_image(t_ip2arr *pixels, t_dp2arr *view,
				   int window_width, int window_height)
{
	int		offset_x;
	int		offset_y;
	t_dp2	offset;
	int		i;
	int		j;
	double	max_x;
	double	max_y;
	double	min_x;
	double	min_y;
	double	width_x;
	double	width_y;
	double	multiplier_x;
	double	multiplier_y;

	rotate_view(view, M_PI);
	min_x = view->points[0][0].x;
	max_x = view->points[0][0].x;
	min_y = view->points[0][0].y;
	max_y = view->points[0][0].y;
	i = -1;
	while (++i < view->rows)
	{
		j = -1;
		while (++j < view->cols)
		{
			if (min_x > view->points[i][j].x)
				min_x = view->points[i][j].x;
			if (max_x < view->points[i][j].x)
				max_x = view->points[i][j].x;
			if (min_y > view->points[i][j].y)
				min_y = view->points[i][j].y;
			if (max_y < view->points[i][j].y)
				max_y = view->points[i][j].y;
		}
	}
	width_x = max_x - min_x;
	width_y = max_y - min_y;
	if (width_x == 0)
		multiplier_x = 100;
	else
		multiplier_x = WIDTH / width_x;
	if (width_y == 0)
		multiplier_y = 100;
	else
		multiplier_y = WIDTH / width_x;
	if (multiplier_x < multiplier_y)
		scale_view(view, multiplier_x);
	else
		scale_view(view, multiplier_y);
	offset_x = window_width / 2;
	offset_y = window_height / 2;
	offset = (t_dp2){.x = offset_x, .y = offset_y};
	offset_view(view, &offset);
	i = -1;
	while (++i < pixels->rows)
	{
		j = -1;
		while (++j < pixels->cols)
		{
			pixels->points[i][j].x = (int)view->points[i][j].x;
			pixels->points[i][j].y = (int)view->points[i][j].y;
		}
	}
}

int		create_pixels(t_ip2arr *pixels, const t_imatrix *map)
{
	int	i;

	pixels->rows = map->rows;
	pixels->cols = map->cols;
	pixels->points = ft_calloc(map->rows, sizeof(t_ip2 *));
	if (!pixels->points)
		return (FAILURE);
	i = -1;
	while (++i < map->rows)
	{
		pixels->points[i] = ft_calloc(map->cols, sizeof(t_ip2));
		if (!pixels->points[i])
		{
			free_pixels(pixels);
			return (FAILURE);
		}
	}
	return (SUCCESS);
}

void	free_pixels(t_ip2arr *pixels)
{
	if (pixels->rows)
		while (pixels->rows--)
			free(pixels->points[pixels->rows]);
	free(pixels->points);
	pixels->points = NULL;
	pixels->cols = 0;
	pixels->rows = 0;
}

void	draw_map(t_ip2arr *pix, t_imatrix *map, mlx_image_t *img)
{
	int	i;
	int	j;

	i = -1;
	while (++i < map->rows - 1)
	{
		j = -1;
		while (++j < map->cols - 1)
		{
			draw_segment(img, pix->points[i][j], pix->points[i + 1][j], WHITE);
			draw_segment(img, pix->points[i][j], pix->points[i][j + 1], WHITE);
		}
	}
	j = map->cols -1;
	i = -1;
	while (++i < map->rows - 1)
		draw_segment(img, pix->points[i][j], pix->points[i + 1][j], WHITE);
	i = map->rows - 1;
	j = -1;
	while (++j < map->cols - 1)
		draw_segment(img, pix->points[i][j], pix->points[i][j + 1], WHITE);
}
