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

void		draw_segment(mlx_image_t *img, t_p2 p1, t_p2 p2, int rgba);
void		fill_with_color(mlx_image_t *img, int rgba);
void		draw_test_reticle(mlx_image_t *img, int rgba);
t_linvars	calculate_linvars(t_p2 p1, t_p2 p2);
int			create_points(t_p3arr *points_array, t_matrix *matrix);
void		assign_points(t_p3arr *points_array, t_matrix *matrix);
void		free_points(t_p3arr *points_array);

int	main(int argc, char *argv[])
{
	mlx_t		*mlx;
	mlx_image_t	*img;
	t_matrix	map;
	t_p3arr		points;
	int			fd;

	if (argc != 2)
		return (write_error_return_int("Usage: ./fdf <file_name>", 1));
	ft_memset(&map, 0, sizeof(map));
	if (validate_map_file(argv[1]))
		return (write_error_return_int("ERROR: invalid map file, "
								"check the file name and/or formatting", 1));
	if (parse_map_file(argv[1], &map) == FAIL)
		return (write_error_return_int("ERROR: failed to parse file", 1));
	print_matrix(&map);
	create_points(&points, &map);
	mlx = mlx_init(WIDTH, HEIGHT, "fdf", 1);
	if (!mlx)
		return (1);
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

void	draw_segment(mlx_image_t *img, t_p2 p1, t_p2 p2, int rgba)
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
		if (x >= 0 && x < img->width && y >= 0 && y < img->height)
			mlx_put_pixel(img, x, y, rgba);
	}
}

t_linvars	calculate_linvars(t_p2 p1, t_p2 p2)
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
	t_p2	p1;
	t_p2	p2;

	p1 = (t_p2){.x = 0, .y = 0};
	p2 = (t_p2){.x = 0, .y = 0};
	draw_segment(img, p1, p2, rgba);
	p1 = (t_p2){.x = WIDTH - 1, .y = HEIGHT - 1};
	p2 = (t_p2){.x = 0, .y = 0};
	draw_segment(img, p1, p2, rgba);
	p1 = (t_p2){.x = 0, .y = 0};
	p2 = (t_p2){.x = WIDTH - 1, .y = HEIGHT - 1};
	draw_segment(img, p1, p2, rgba);
	p1 = (t_p2){.x = 0, .y = HEIGHT - 1};
	p2 = (t_p2){.x = WIDTH - 1, .y = 0};
	draw_segment(img, p1, p2, rgba);
	p1 = (t_p2){.x = WIDTH - 1, .y = 0};
	p2 = (t_p2){.x = 0, .y = HEIGHT - 1};
	draw_segment(img, p1, p2, rgba);
	p1 = (t_p2){.x = WIDTH / 2, .y = 0};
	p2 = (t_p2){.x = WIDTH / 2, .y = HEIGHT - 1};
	draw_segment(img, p1, p2, rgba);
	p1 = (t_p2){.x = WIDTH / 2, .y = HEIGHT - 1};
	p2 = (t_p2){.x = WIDTH / 2, .y = 0};
	draw_segment(img, p1, p2, rgba);
	p1 = (t_p2){.x = 0, .y = HEIGHT / 2};
	p2 = (t_p2){.x = WIDTH - 1, .y = HEIGHT / 2};
	draw_segment(img, p1, p2, rgba);
	p1 = (t_p2){.x = WIDTH - 1, .y = HEIGHT / 2};
	p2 = (t_p2){.x = 0, .y = HEIGHT / 2};
	draw_segment(img, p1, p2, rgba);
	p1 = (t_p2){.x = WIDTH / 2, .y = 0};
	p2 = (t_p2){.x = 0, .y = HEIGHT / 2};
	draw_segment(img, p1, p2, rgba);
	p1 = (t_p2){.x = 0, .y = HEIGHT / 2};
	p2 = (t_p2){.x = WIDTH / 2, .y = 0};
	draw_segment(img, p1, p2, rgba);
	p1 = (t_p2){.x = WIDTH - 1, .y = HEIGHT / 2};
	p2 = (t_p2){.x = WIDTH / 2, .y = 0};
	draw_segment(img, p1, p2, rgba);
	p1 = (t_p2){.x = WIDTH / 2, .y = 0};
	p2 = (t_p2){.x = WIDTH - 1, .y = HEIGHT / 2};
	draw_segment(img, p1, p2, rgba);
	p1 = (t_p2){.x = WIDTH / 2, .y = HEIGHT - 1};
	p2 = (t_p2){.x = 0, .y = HEIGHT / 2};
	draw_segment(img, p1, p2, rgba);
	p1 = (t_p2){.x = 0, .y = HEIGHT / 2};
	p2 = (t_p2){.x = WIDTH / 2, .y = HEIGHT - 1};
	draw_segment(img, p1, p2, rgba);
	p1 = (t_p2){.x = WIDTH / 2, .y = HEIGHT - 1};
	p2 = (t_p2){.x = WIDTH - 1, .y = HEIGHT / 2};
	draw_segment(img, p1, p2, rgba);
	p1 = (t_p2){.x = WIDTH - 1, .y = HEIGHT / 2};
	p2 = (t_p2){.x = WIDTH / 2, .y = HEIGHT - 1};
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
	while (i < w * h)
	{
		mlx_put_pixel(img, i % w, (i / w) % h, rgba);
		i++;
	}
}

int	create_points(t_p3arr *points_array, t_matrix *matrix)
{
	int	i;

	points_array->rows = matrix->rows;
	points_array->cols = matrix->cols;
	points_array->points = ft_calloc(points_array->rows, sizeof(t_p3 *));
	if (!points_array->points)
		return (FAIL);
	i = -1;
	while (++i < points_array->rows)
	{
		points_array->points[i] = malloc(points_array->cols * sizeof(t_p3));
		if (!points_array->points[i])
		{
			free_points(points_array);
			return (FAIL);
		}
	}
	assign_points(points_array, matrix);
	return (SUCCESS);
}

void	assign_points(t_p3arr *points_array, t_matrix *matrix)
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

void	free_points(t_p3arr *points_array)
{
	if (points_array->rows)
		while (points_array->rows--)
			free(points_array->points[points_array->rows]);
	free(points_array->points);
	points_array->points = NULL;
	points_array->cols = 0;
	points_array->rows = 0;
}
