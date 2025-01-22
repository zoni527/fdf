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

void		draw_segment(t_fdf_data *data, t_pixel p1, t_pixel p2);
void		fill_with_color(mlx_image_t *img, unsigned int rgba);
void		draw_test_reticle(mlx_image_t *img, int rgba);
t_linvars	calculate_linvars(t_pixel p1, t_pixel p2);

void		assign_world(t_fdf_data *data);
void		free_points(t_fdf_data *data);

void		matrix_transform(t_dp3 *p, double **t);

void		offset_dp3(t_dp3 *point, t_dp3 offset);
void		scale_dp3(t_dp3 *point, double multiplier);
void		rotate_dp3(t_dp3 *point, double rx, double ry, double rz);

void		offset_world(t_fdf_data *data, t_dp3 offset);
void		scale_world(t_fdf_data *data, double multiplier);
void		rotate_world(t_fdf_data *data, double rx, double ry, double rz);
void		center_world(t_fdf_data *data);

void		y_plane_projection(t_fdf_data *data);
void		x_plane_projection(t_fdf_data *data);
void		z_plane_projection(t_fdf_data *data);

void		offset_dp2(t_dp2 *point, t_dp2 offset);
void		rotate_dp2(t_dp2 *point, double rx, double ry, double rz);
void		scale_dp2(t_dp2 *point, double multiplier);

void		offset_view(t_fdf_data *data, t_dp2 offset);
void		scale_view(t_fdf_data *data, double multiplier);
void		rotate_view(t_fdf_data *data, double rot);

double		min_x_view(t_fdf_data *data);
double		max_x_view(t_fdf_data *data);
double		min_y_view(t_fdf_data *data);
double		max_y_view(t_fdf_data *data);

void		center_view(t_fdf_data *data);
void		stretch_view_to_window_scale(t_fdf_data *data);
void		initialize_pixels(t_fdf_data *data);
void		assign_pixels(t_fdf_data *data);

void		draw_map(t_fdf_data *data);

void		free_data(t_fdf_data *data);
void		free_close_print_exit(t_fdf_data *data, const char *error);

void		set_up_scene(t_fdf_data *data);

int	main(int argc, char *argv[])
{
	t_fdf_data	data;

	if (argc != 2)
		return (write_error_return_int("Usage: ./fdf <map_name>.fdf", 1));
	ft_memset(&data, 0, sizeof(t_fdf_data));
	if (validate_map_file(argv[1], &data) == FAILURE)
		free_close_print_exit(&data, "ERROR: couldn't validate map file");
	parse_map_file(argv[1], &data);
	set_up_scene(&data);
	fill_with_color(data.img, BLACK);
	draw_map(&data);
	mlx_loop(data.mlx);
	mlx_terminate(data.mlx);
	free_data(&data);
	return (0);
}

void	set_up_scene(t_fdf_data *data)
{
	print_map(data);
	print_colors(data);
	assign_world(data);
	center_world(data);
	y_plane_projection(data);
	initialize_pixels(data);
	data->mlx = mlx_init(WIDTH, HEIGHT, "fdf", 1);
	if (!data->mlx)
		free_close_print_exit(data, "ERROR: couldn't initiate mlx");
	data->img = mlx_new_image(data->mlx, WIDTH, HEIGHT);
	if (!data->img)
		free_close_print_exit(data, "ERROR: couldn't create image");
	if (mlx_image_to_window(data->mlx, data->img, 0, 0) < 0)
		free_close_print_exit(data, "ERROR: couldn't insert image to window");
}

void	free_close_print_exit(t_fdf_data *data, const char *error)
{
	free_data(data);
	if (data->fd > 0)
		close(data->fd);
	if (data->mlx)
		mlx_terminate(data->mlx);
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
		if (data->world)
			free(data->world[i]);
		if (data->view)
			free(data->view[i]);
		if (data->pixels)
			free(data->pixels[i]);
	}
	free(data->map);
	free(data->world);
	free(data->view);
	free(data->pixels);
}

void	draw_segment(t_fdf_data *data, t_pixel p1, t_pixel p2)
{
	t_linvars		l;
	int				i;
	int				x;
	int				y;
	int				r1;
	int				r2;
	int				r;
	int				g1;
	int				g2;
	int				g;
	int				b1;
	int				b2;
	int				b;
	unsigned int	rgba;

	r1 = p1.rgba >> 24 & 0xff;
	r2 = p2.rgba >> 24 & 0xff;
	g1 = p1.rgba >> 16 & 0xff;
	g2 = p2.rgba >> 16 & 0xff;
	b1 = p1.rgba >> 8 & 0xff;
	b2 = p2.rgba >> 8 & 0xff;
	l = calculate_linvars(p1, p2);
	i = -1;
	if (l.delta_x == 0)
	{
		while (++i < abs(l.delta_y) + 1)
		{
			x = p1.x;
			y = (int)(p1.y + i * l.step_y);
			r = (int)(r1 + i * (double)(r2 - r1) / abs(l.delta_y));
			g = (int)(g1 + i * (double)(g2 - g1) / abs(l.delta_y));
			b = (int)(b1 + i * (double)(b2 - b1) / abs(l.delta_y));
			rgba = (r << 24 | g << 16 | b << 8 | 0xff);
			if (x >= 0 && x < (int)data->img->width && y >= 0 && y < (int)data->img->height)
				mlx_put_pixel(data->img, p1.x, (int)(p1.y + i * l.step_y), rgba);
		}
		return ;
	}
	if (l.slope <= 1)
	{
		while (++i < ft_iabs(l.delta_x) + 1)
		{
			x = p1.x + i * l.step_x;
			y = (int)(p1.y + i * l.step_y * l.slope);
			r = (int)(r1 + i * (double)(r2 - r1) / abs(l.delta_x));
			g = (int)(g1 + i * (double)(g2 - g1) / abs(l.delta_x));
			b = (int)(b1 + i * (double)(b2 - b1) / abs(l.delta_x));
			rgba = (r << 24 | g << 16 | b << 8 | 0xff);
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
			r = (int)(r1 + i * (double)(r2 - r1) / abs(l.delta_y));
			g = (int)(g1 + i * (double)(g2 - g1) / abs(l.delta_y));
			b = (int)(b1 + i * (double)(b2 - b1) / abs(l.delta_y));
			rgba = (r << 24 | g << 16 | b << 8 | 0xff);
			if (x >= 0 && x < (int)data->img->width && y >= 0 && y < (int)data->img->height)
				mlx_put_pixel(data->img, x, y, rgba);
		}
	}
}

void	draw_vertical_segment(t_pixel p1, t_pixel p2, t_linvars *l)
{
	int	x;
	int	y;
	int	rgba;
	int	i;

	i = -1;
	while (++i < abs(l->delta_y) + 1)
	{
		x = p1.x;
		y = (int)(p1.y + i * l.step_y);
		r = (int)(r1 + i * (double)(r2 - r1) / abs(l->delta_y));
		g = (int)(g1 + i * (double)(g2 - g1) / abs(l->delta_y));
		b = (int)(b1 + i * (double)(b2 - b1) / abs(l->delta_y));
		rgba = (r << 24 | g << 16 | b << 8 | 0xff);
		if (x >= 0 && x < (int)data->img->width && y >= 0 && y < (int)data->img->height)
			mlx_put_pixel(data->img, p1.x, (int)(p1.y + i * l->step_y), rgba);
	}
}

int	interpolate_color_channel(int p1_c, int p2_c, double distance, int index)
{
	return ((int)(p1_c + index * (double)(p2_c - p1_c) / distance));
}

t_linvars	calculate_linvars(t_pixel p1, t_pixel p2)
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
	t_pixel	p1;
	t_pixel	p2;

	p1 = (t_pixel){.x = 0, .y = 0};
	p2 = (t_pixel){.x = 0, .y = 0};
	draw_segment(img, p1, p2, rgba);
	p1 = (t_pixel){.x = WIDTH - 1, .y = HEIGHT - 1};
	p2 = (t_pixel){.x = 0, .y = 0};
	draw_segment(img, p1, p2, rgba);
	p1 = (t_pixel){.x = 0, .y = 0};
	p2 = (t_pixel){.x = WIDTH - 1, .y = HEIGHT - 1};
	draw_segment(img, p1, p2, rgba);
	p1 = (t_pixel){.x = 0, .y = HEIGHT - 1};
	p2 = (t_pixel){.x = WIDTH - 1, .y = 0};
	draw_segment(img, p1, p2, rgba);
	p1 = (t_pixel){.x = WIDTH - 1, .y = 0};
	p2 = (t_pixel){.x = 0, .y = HEIGHT - 1};
	draw_segment(img, p1, p2, rgba);
	p1 = (t_pixel){.x = WIDTH / 2, .y = 0};
	p2 = (t_pixel){.x = WIDTH / 2, .y = HEIGHT - 1};
	draw_segment(img, p1, p2, rgba);
	p1 = (t_pixel){.x = WIDTH / 2, .y = HEIGHT - 1};
	p2 = (t_pixel){.x = WIDTH / 2, .y = 0};
	draw_segment(img, p1, p2, rgba);
	p1 = (t_pixel){.x = 0, .y = HEIGHT / 2};
	p2 = (t_pixel){.x = WIDTH - 1, .y = HEIGHT / 2};
	draw_segment(img, p1, p2, rgba);
	p1 = (t_pixel){.x = WIDTH - 1, .y = HEIGHT / 2};
	p2 = (t_pixel){.x = 0, .y = HEIGHT / 2};
	draw_segment(img, p1, p2, rgba);
	p1 = (t_pixel){.x = WIDTH / 2, .y = 0};
	p2 = (t_pixel){.x = 0, .y = HEIGHT / 2};
	draw_segment(img, p1, p2, rgba);
	p1 = (t_pixel){.x = 0, .y = HEIGHT / 2};
	p2 = (t_pixel){.x = WIDTH / 2, .y = 0};
	draw_segment(img, p1, p2, rgba);
	p1 = (t_pixel){.x = WIDTH - 1, .y = HEIGHT / 2};
	p2 = (t_pixel){.x = WIDTH / 2, .y = 0};
	draw_segment(img, p1, p2, rgba);
	p1 = (t_pixel){.x = WIDTH / 2, .y = 0};
	p2 = (t_pixel){.x = WIDTH - 1, .y = HEIGHT / 2};
	draw_segment(img, p1, p2, rgba);
	p1 = (t_pixel){.x = WIDTH / 2, .y = HEIGHT - 1};
	p2 = (t_pixel){.x = 0, .y = HEIGHT / 2};
	draw_segment(img, p1, p2, rgba);
	p1 = (t_pixel){.x = 0, .y = HEIGHT / 2};
	p2 = (t_pixel){.x = WIDTH / 2, .y = HEIGHT - 1};
	draw_segment(img, p1, p2, rgba);
	p1 = (t_pixel){.x = WIDTH / 2, .y = HEIGHT - 1};
	p2 = (t_pixel){.x = WIDTH - 1, .y = HEIGHT / 2};
	draw_segment(img, p1, p2, rgba);
	p1 = (t_pixel){.x = WIDTH - 1, .y = HEIGHT / 2};
	p2 = (t_pixel){.x = WIDTH / 2, .y = HEIGHT - 1};
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

void	matrix_transform(t_dp3 *p, double **t)
{
	p->x = t[0][0] * p->x + t[0][1] * p->y + t[0][2] * p->z;
	p->y = t[1][0] * p->x + t[1][1] * p->y + t[1][2] * p->z;
	p->z = t[2][0] * p->x + t[2][1] * p->y + t[2][2] * p->z;
}

void	rotate_dp3(t_dp3 *point, double rx, double ry, double rz)
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
			rotate_dp3(&data->world[i][j], rx, ry, rz);
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
			scale_dp3(&data->world[i][j], multiplier);
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
			offset_dp3(&data->world[i][j], offset);
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
	scale_dp3(&offset, -1);
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

void	x_plane_projection(t_fdf_data *data)
{
	int	i;
	int	j;

	i = -1;
	while (++i < data->rows)
	{
		j = -1;
		while (++j < data->cols)
		{
			data->view[i][j].x = data->world[i][j].y;
			data->view[i][j].y = data->world[i][j].z;
		}
	}
}

void	z_plane_projection(t_fdf_data *data)
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
			data->view[i][j].y = data->world[i][j].y;
		}
	}
}

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

void	offset_view(t_fdf_data *data, t_dp2 offset)
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

void	scale_view(t_fdf_data *data, double multiplier)
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

void	center_view_x(t_fdf_data *data)
{
	double	min_x;
	double	max_x;
	int		i;
	int		j;
	t_dp2	offset;

	min_x = data->view[0][0].x;
	max_x = data->view[0][0].x;
	i = -1;
	while (++i < data->rows)
	{
		j = -1;
		while (++j < data->cols)
		{
			if (data->view[i][j].x < min_x)
				min_x = data->view[i][j].x;
			if (data->view[i][j].x > max_x)
				max_x = data->view[i][j].x;
		}
	}
	offset.x = -(max_x + min_x) / 2;
	offset.y = 0;
	offset_view(data, offset);
}

double	min_y_view(t_fdf_data *data)
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

double	max_y_view(t_fdf_data *data)
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

double	min_x_view(t_fdf_data *data)
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

double	max_x_view(t_fdf_data *data)
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

void	center_view_y(t_fdf_data *data)
{
	double	min_y;
	double	max_y;
	int		i;
	int		j;
	t_dp2	offset;

	min_y = data->view[0][0].y;
	max_y = data->view[0][0].y;
	i = -1;
	while (++i < data->rows)
	{
		j = -1;
		while (++j < data->cols)
		{
			if (data->view[i][j].y < min_y)
				min_y = data->view[i][j].y;
			if (data->view[i][j].y > max_y)
				max_y = data->view[i][j].y;
		}
	}
	offset.y = -(max_y + min_y) / 2;
	offset.x = 0;
	offset_view(data, offset);
}

void	center_view(t_fdf_data *data)
{
	center_view_x(data);
	center_view_y(data);
}


void	initialize_pixels(t_fdf_data *data)
{
	t_dp2	offset;

	rotate_view(data, M_PI);
	center_view(data);
	stretch_view_to_window_scale(data);
	offset.x = (double)WIDTH / 2;
	offset.y = (double)HEIGHT / 2;
	data->view_offset = offset;
	offset_view(data, data->view_offset);
	assign_pixels(data);
}

void	stretch_view_to_window_scale(t_fdf_data *data)
{
	double	width_x;
	double	width_y;
	double	ratio_x;
	double	ratio_y;

	ratio_x = 1;
	ratio_y = 1;
	data->view_scale = 1;
	width_x = max_x_view(data) - min_x_view(data);
	width_y = max_y_view(data) - min_y_view(data);
	if (width_y == 0 && width_x == 0)
		return ;
	if (width_x != 0)
		ratio_x = WIDTH / width_x;
	if (width_y != 0)
		ratio_y = WIDTH / width_y;
	if (ratio_x == 1)
		data->view_scale = ratio_y;
	if (ratio_y == 1)
		data->view_scale = ratio_x;
	if (ratio_y < ratio_x)
		data->view_scale = ratio_y;
	else
		data->view_scale = ratio_x;
	scale_view(data, data->view_scale);
}

void	assign_pixels(t_fdf_data *data)
{
	int	i;
	int	j;

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
			draw_segment(data, data->pixels[i][j], data->pixels[i + 1][j]);
			draw_segment(data, data->pixels[i][j], data->pixels[i][j + 1]);
		}
	}
	j = data->cols -1;
	i = -1;
	while (++i < data->rows - 1)
		draw_segment(data, data->pixels[i][j], data->pixels[i + 1][j]);
	i = data->rows - 1;
	j = -1;
	while (++j < data->cols - 1)
		draw_segment(data, data->pixels[i][j], data->pixels[i][j + 1]);
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
			ft_printf("0x%x ", data->pixels[i][j].rgba);
		}
		ft_printf("\n");
	}
}
