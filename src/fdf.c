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

void	set_default_gradient(t_fdf *data);

int	main(int argc, char *argv[])
{
	t_fdf	data;

	if (argc != 2)
		return (write_error_return_int("Usage: ./fdf <map_name>.fdf", 1));
	ft_memset(&data, 0, sizeof(t_fdf));
	if (validate_map_file(argv[1], &data) == FAILURE)
		free_close_print_exit(&data, "ERROR: couldn't validate map file");
	allocate_data(&data);
	parse_map_file(argv[1], &data);
	set_up_scene(&data);
	draw_map(&data);
	mlx_loop_hook(data.mlx, &hook, &data);
	mlx_scroll_hook(data.mlx, &scroll_hook, &data);
	mlx_loop(data.mlx);
	mlx_terminate(data.mlx);
	free_data(&data);
	return (0);
}

void	set_up_scene(t_fdf *data)
{
	print_map(data);
	print_colors(data);
	assign_world(data);
	center_world(data);
	set_default_gradient(data);
	rotate_world(data, 0, 0, 180 * ONE_DEGREE);
	rotate_world(data, 0, 0, 45 * ONE_DEGREE);
	rotate_world(data, atan(1 / sqrt(2)), 0, 0);
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
	fill_with_color(data->img, BLACK);
}

void	free_close_print_exit(t_fdf *data, const char *error)
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

void	allocate_data(t_fdf *data)
{
	int	i;

	data->map = ft_calloc(data->rows, sizeof(int *));
	data->world = ft_calloc(data->rows, sizeof(t_dp3 *));
	data->view = ft_calloc(data->rows, sizeof(t_dp2 *));
	data->pixels = ft_calloc(data->rows, sizeof(t_pixel *));
	if (!data->map || !data->world || !data->view || !data->pixels)
		free_close_print_exit(data, "ERROR: couldn't allocate data");
	i = -1;
	while (++i < data->rows)
	{
		data->map[i] = ft_calloc(data->cols, sizeof(int));
		data->world[i] = ft_calloc(data->cols, sizeof(t_dp3));
		data->view[i] = ft_calloc(data->cols, sizeof(t_dp2));
		data->pixels[i] = ft_calloc(data->cols, sizeof(t_pixel));
		if (!data->map[i] || !data->world[i]
			|| !data->view[i] || !data->pixels[i])
			free_close_print_exit(data, "ERROR: couldn't allocate data");
	}
}

void	free_data(t_fdf *data)
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

void	set_default_gradient(t_fdf *data)
{
	double	min_z;
	double	max_z;
	unsigned int	color_1;
	unsigned int	color_2;
	double	a;
	double	b;
	double	z;
	size_t	i;

	color_1 = LINE_GRADIENT_COLOR_1;
	color_2 = LINE_GRADIENT_COLOR_2;
	min_z = min_z_world(data);
	max_z = max_z_world(data);
	a = ((double)color_1 - color_2) / ((double)min_z - max_z);
	b = ((double)color_2 * min_z - color_1 * max_z) / ((double)min_z - max_z);
	i = 0;
	while (i < (size_t)(data->rows * data->cols))
	{
		if (data->pixels[i / data->cols][i % data->cols].rgba == 0xffffffff)
		{
			z = data->world[i / data->cols][i % data->cols].z;
			data->pixels[i / data->cols][i % data->cols].rgba = (unsigned int)(a * z + b);
		}
		i++;
	}
}
