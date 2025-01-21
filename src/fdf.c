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

void		draw_segment(t_fdf_data *data, t_ip2 p1, t_ip2 p2,
				   unsigned int rgba);
void		fill_with_color(mlx_image_t *img, unsigned int rgba);
void		draw_test_reticle(mlx_image_t *img, int rgba);
t_linvars	calculate_linvars(t_ip2 p1, t_ip2 p2);

void		assign_world(t_fdf_data *data);
void		free_points(t_fdf_data *data);

void		matrix_transform(t_dp3 *p, double **t);

void		offset_point(t_dp3 *point, t_dp3 offset);
void		rotate_point(t_dp3 *point, double rx, double ry, double rz);
void		scale_point(t_dp3 *point, double multiplier);

void		offset_world(t_fdf_data *data, t_dp3 offset);
void		scale_world(t_fdf_data *data, double multiplier);
void		rotate_world(t_fdf_data *data, double rx, double ry, double rz);
void		center_world(t_fdf_data *data);

void		y_plane_projection(t_fdf_data *data);

void		rotate_view(t_fdf_data *data, double rot);
void		scale_view(t_fdf_data *data, double multiplier);
void		offset_view(t_fdf_data *data, t_dp2 *offset);
void		view_to_image(t_fdf_data *data, int win_width, int win_height);

void		draw_map(t_fdf_data *data);

void		free_data(t_fdf_data *data);
void		free_print_exit(t_fdf_data *data, const char *error);

int	main(int argc, char *argv[])
{
	t_fdf_data	data;

	if (argc != 2)
		return (write_error_return_int("Usage: ./fdf <file_name>", 1));
	ft_memset(&data, 0, sizeof(t_fdf_data));
	if (validate_map_file(argv[1]) == FAILURE)
		return (write_error_return_int("ERROR: invalid map file, "
				"check the file name and/or formatting", 1));
	parse_map_file(argv[1], &data);
	print_map(&data);
	print_colors(&data);
	assign_world(&data);
	center_world(&data);
	rotate_world(&data, M_PI / 6, M_PI / 6, M_PI / 6);
	scale_world(&data, 0.5);
	y_plane_projection(&data);
	view_to_image(&data, WIDTH, HEIGHT);
	data.mlx = mlx_init(WIDTH, HEIGHT, "fdf", 1);
	if (!data.mlx)
		free_print_exit(&data, "ERROR: couldn't initiate mlx");
	data.img = mlx_new_image(data.mlx, WIDTH, HEIGHT);
	if (!data.img)
		free_print_exit(&data, "ERROR: couldn't create image");
	if (mlx_image_to_window(data.mlx, data.img, 0, 0) < 0)
		free_print_exit(&data, "ERROR: couldn't draw image");
	fill_with_color(data.img, BLACK);
	draw_map(&data);
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
	int	i;

	i = -1;
	while (++i < data->rows)
	{
		if (data->map)
			free(data->map[i]);
		if (data->colors)
			free(data->colors[i]);
		if (data->world)
			free(data->world[i]);
		if (data->view)
			free(data->view[i]);
		if (data->pixels)
			free(data->pixels[i]);
	}
	free(data->map);
	free(data->colors);
	free(data->world);
	free(data->view);
	free(data->pixels);
}

void	draw_segment(t_fdf_data *data, t_ip2 p1, t_ip2 p2,
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
			mlx_put_pixel(data->img, p1.x, (int)(p1.y + i * l.step_y), rgba);
		return ;
	}
	if (l.slope <= 1)
	{
		while (++i < ft_iabs(l.delta_x) + 1)
		{
			x = p1.x + i * l.step_x;
			y = (int)(p1.y + i * l.step_y * l.slope);
			if (x >= 0 && x < (int)data->img->width && y >= 0 && y < (int)data->img->height)
				mlx_put_pixel(data->img, x, y, rgba);
		}
	}
	else
	{
		while (++i < ft_iabs(l.delta_y) + 1)
		{
			y = p1.y + i * l.step_y;
			x = (int)(p1.x + i * l.step_x * 1 / l.slope);
			if (x >= 0 && x < (int)data->img->width && y >= 0 && y < (int)data->img->height)
				mlx_put_pixel(data->img, x, y, rgba);
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

/*
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
*/

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

void	assign_world(t_fdf_data *data)
{
	int	i;
	int	j;

	i = -1;
	while (++i < data->rows)
	{
		j = -1;
		while (++j < data->cols)
		{
			data->world[i][j].x = j;
			data->world[i][j].y = i;
			data->world[i][j].z = data->map[i][j];
		}
	}
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

void	matrix_transform(t_dp3 *p, double **t)
{
	p->x = t[0][0] * p->x + t[0][1] * p->y + t[0][2] * p->z;
	p->y = t[1][0] * p->x + t[1][1] * p->y + t[1][2] * p->z;
	p->z = t[2][0] * p->x + t[2][1] * p->y + t[2][2] * p->z;
}

void	rotate_point(t_dp3 *point, double rx, double ry, double rz)
{
	double	**x_rot;
	double	**y_rot;
	double	**z_rot;

	x_rot = (double *[]){
	(double []){1, 0, 0},
	(double []){0, cos(rx), -sin(rx)},
	(double []){0, sin(rx), cos(rx)}
	};
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

void	rotate_world(t_fdf_data *data, double rx, double ry, double rz)
{
	int	i;
	int	j;

	i = -1;
	while (++i < data->rows)
	{
		j = -1;
		while (++j < data->cols)
			rotate_point(&data->world[i][j], rx, ry, rz);
	}
}

void	scale_world(t_fdf_data *data, double multiplier)
{
	int	i;
	int	j;

	i = -1;
	while (++i < data->rows)
	{
		j = -1;
		while (++j < data->cols)
			scale_point(&data->world[i][j], multiplier);
	}
}

void	offset_world(t_fdf_data *data, t_dp3 offset)
{
	int	i;
	int	j;

	i = -1;
	while (++i < data->rows)
	{
		j = -1;
		while (++j < data->cols)
			offset_point(&data->world[i][j], offset);
	}
}

void	center_world(t_fdf_data *data)
{
	double	min_x;
	double	max_x;
	double	min_y;
	double	max_y;
	double	min_z;
	double	max_z;
	t_dp3	offset;
	size_t	i;

	offset = data->world[0][0];
	scale_point(&offset, -1);
	offset_world(data, offset);
	min_x = data->world[0][0].x;
	max_x = data->world[0][0].x;
	min_y = data->world[0][0].y;
	max_y = data->world[0][0].y;
	min_z = data->world[0][0].z;
	max_z = data->world[0][0].z;
	i = 1;
	while (i < (size_t)(data->cols * data->rows))
	{
		if (data->world[i / data->cols][i % data->cols].x < min_x)
			min_x = data->world[i / data->cols][i % data->cols].x;
		if (data->world[i / data->cols][i % data->cols].x > max_x)
			max_x = data->world[i / data->cols][i % data->cols].x;
		if (data->world[i / data->cols][i % data->cols].y < min_y)
			min_y = data->world[i / data->cols][i % data->cols].y;
		if (data->world[i / data->cols][i % data->cols].y > max_y)
			max_y = data->world[i / data->cols][i % data->cols].y;
		if (data->world[i / data->cols][i % data->cols].z < min_z)
			min_z = data->world[i / data->cols][i % data->cols].z;
		if (data->world[i / data->cols][i % data->cols].z > max_z)
			max_z = data->world[i / data->cols][i % data->cols].z;
		i++;
	}
	offset = (t_dp3){
		.x = - (max_x + min_x) / 2,
		.y = - (max_y + min_y) / 2,
		.z = - (max_z + min_z) / 2};
	offset_world(data, offset);
}

void	y_plane_projection(t_fdf_data *data)
{
	int	i;
	int	j;

	i = -1;
	while (++i < data->rows)
	{
		j = -1;
		while (++j < data->cols)
		{
			data->view[i][j].x = data->world[i][j].x;
			data->view[i][j].y = data->world[i][j].z;
		}
	}
}

void	rotate_view(t_fdf_data *data, double rot)
{
	int		i;
	int		j;
	double	x;
	double	y;

	i = -1;
	while (++i < data->rows)
	{
		j = -1;
		while (++j < data->cols)
		{
			x = data->view[i][j].x;
			y = data->view[i][j].y;
			data->view[i][j].x = x * cos(rot) - y * sin(rot);
			data->view[i][j].y = x * sin(rot) + y * cos(rot);
		}
	}
}

void	scale_view(t_fdf_data *data, double multiplier)
{

	int		i;
	int		j;

	i = -1;
	while (++i < data->rows)
	{
		j = -1;
		while (++j < data->cols)
		{
			data->view[i][j].x *= multiplier;
			data->view[i][j].y *= multiplier;
		}
	}
}

void	offset_view(t_fdf_data *data, t_dp2 *offset)
{
	int		i;
	int		j;

	i = -1;
	while (++i < data->rows)
	{
		j = -1;
		while (++j < data->cols)
		{
			data->view[i][j].x += offset->x;
			data->view[i][j].y += offset->y;
		}
	}
}

void	view_to_image(t_fdf_data *data, int win_width, int win_height)
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

	rotate_view(data, M_PI);
	min_x = data->view[0][0].x;
	max_x = data->view[0][0].x;
	min_y = data->view[0][0].y;
	max_y = data->view[0][0].y;
	i = -1;
	while (++i < data->rows)
	{
		j = -1;
		while (++j < data->cols)
		{
			if (min_x > data->view[i][j].x)
				min_x = data->view[i][j].x;
			if (max_x < data->view[i][j].x)
				max_x = data->view[i][j].x;
			if (min_y > data->view[i][j].y)
				min_y = data->view[i][j].y;
			if (max_y < data->view[i][j].y)
				max_y = data->view[i][j].y;
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
		scale_view(data, multiplier_x);
	else
		scale_view(data, multiplier_y);
	offset_x = win_width / 2;
	offset_y = win_height / 2;
	offset = (t_dp2){.x = offset_x, .y = offset_y};
	offset_view(data, &offset);
	i = -1;
	while (++i < data->rows)
	{
		j = -1;
		while (++j < data->cols)
		{
			data->pixels[i][j].x = (int)data->view[i][j].x;
			data->pixels[i][j].y = (int)data->view[i][j].y;
		}
	}
}

void	draw_map(t_fdf_data *data)
{
	int	i;
	int	j;

	i = -1;
	while (++i < data->rows - 1)
	{
		j = -1;
		while (++j < data->cols - 1)
		{
			draw_segment(data, data->pixels[i][j], data->pixels[i + 1][j], WHITE);
			draw_segment(data, data->pixels[i][j], data->pixels[i][j + 1], WHITE);
		}
	}
	j = data->cols -1;
	i = -1;
	while (++i < data->rows - 1)
		draw_segment(data, data->pixels[i][j], data->pixels[i + 1][j], WHITE);
	i = data->rows - 1;
	j = -1;
	while (++j < data->cols - 1)
		draw_segment(data, data->pixels[i][j], data->pixels[i][j + 1], WHITE);
}

void	print_map(t_fdf_data *data)
{
	int	i;
	int	j;

	i = -1;
	while (++i < data->rows)
	{
		j = -1;
		while (++j < data->cols)
		{
			ft_printf("%d ", data->map[i][j]);
		}
		ft_printf("\n");
	}
}

void	print_colors(t_fdf_data *data)
{
	int	i;
	int	j;

	i = -1;
	while (++i < data->rows)
	{
		j = -1;
		while (++j < data->cols)
		{
			ft_printf("%x ", data->colors[i][j]);
		}
		ft_printf("\n");
	}
}
