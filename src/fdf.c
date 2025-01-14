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

void	connect_pixels(mlx_image_t *img, t_point2 p1, t_point2 p2, int color);
void	fill_with_color(mlx_image_t *img, int color);
void	draw_test_reticle(mlx_image_t *img, int color);
int		parse_landscape(t_matrix *land, const char *file_name);
int		count_rows(const char *file_name);
void	free_matrix_elements(t_matrix *m);
void	print_matrix(t_matrix *m);

int	main(int argc, char *argv[])
{
	mlx_t		*mlx;
	mlx_image_t	*img;
	t_point2	p1;
	t_point2	p2;
	t_matrix	landscape;
	int			fd;

	if (argc != 2)
		return (write_error_return_int("Usage: ./fdf <file_name>", 1));
	ft_memset(&landscape, 0, sizeof(landscape));
	if (parse_landscape(&landscape, argv[1]) == FAIL)
	{
		free_matrix_elements(&landscape);
		return (write_error_return_int("ERROR: failed to parse file", 1));
	}
	print_matrix(&landscape);
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
	return (0);
}

void	connect_pixels(mlx_image_t *img, t_point2 p1, t_point2 p2, int color)
{
	int		delta_x;
	int		delta_y;
	double	k;
	int		sx;
	int		sy;
	int		i;
	int		x;
	int		y;

	delta_x = p2.x - p1.x;
	delta_y = p2.y - p1.y;
	sx = 1 + (delta_x < 0) * (-2);
	sy = 1 + (delta_y < 0) * (-2);
	i = -1;
	if (delta_x == 0)
	{
		while (++i < abs(delta_y) + 1)
			mlx_put_pixel(img, p1.x, (int)(p1.y + i * sy), color);
		return ;
	}
	k = fabs((double)delta_y / delta_x);
	while (++i < abs(delta_x) + 1)
	{
		x = p1.x + i * sx;
		y = (int)(p1.y + i * sy * k);
		if (x >= 0 && x < img->width && y >= 0 && y < img->height)
			mlx_put_pixel(img, p1.x + (i * sx), (int)(p1.y + (i * sy) * k), color);
	}
}

void	draw_test_reticle(mlx_image_t *img, int color)
{
	t_point2	p1;
	t_point2	p2;

	p1 = (t_point2){.x = 0, .y = 0};
	p2 = (t_point2){.x = 0, .y = 0};
	connect_pixels(img, p1, p2, color);
	p1 = (t_point2){.x = WIDTH - 1, .y = HEIGHT - 1};
	p2 = (t_point2){.x = 0, .y = 0};
	connect_pixels(img, p1, p2, color);
	p1 = (t_point2){.x = 0, .y = 0};
	p2 = (t_point2){.x = WIDTH - 1, .y = HEIGHT - 1};
	connect_pixels(img, p1, p2, color);
	p1 = (t_point2){.x = 0, .y = HEIGHT - 1};
	p2 = (t_point2){.x = WIDTH - 1, .y = 0};
	connect_pixels(img, p1, p2, color);
	p1 = (t_point2){.x = WIDTH - 1, .y = 0};
	p2 = (t_point2){.x = 0, .y = HEIGHT - 1};
	connect_pixels(img, p1, p2, color);
	p1 = (t_point2){.x = WIDTH/2, .y = 0};
	p2 = (t_point2){.x = WIDTH/2, .y = HEIGHT - 1};
	connect_pixels(img, p1, p2, color);
	p1 = (t_point2){.x = WIDTH/2, .y = HEIGHT - 1};
	p2 = (t_point2){.x = WIDTH/2, .y = 0};
	connect_pixels(img, p1, p2, color);
	p1 = (t_point2){.x = 0, .y = HEIGHT / 2};
	p2 = (t_point2){.x = WIDTH - 1, .y = HEIGHT / 2};
	connect_pixels(img, p1, p2, color);
	p1 = (t_point2){.x = WIDTH - 1, .y = HEIGHT / 2};
	p2 = (t_point2){.x = 0, .y = HEIGHT / 2};
	connect_pixels(img, p1, p2, color);
	p1 = (t_point2){.x = WIDTH / 2, .y = 0};
	p2 = (t_point2){.x = 0, .y = HEIGHT / 2};
	connect_pixels(img, p1, p2, color);
	p1 = (t_point2){.x = 0, .y = HEIGHT / 2};
	p2 = (t_point2){.x = WIDTH / 2, .y = 0};
	connect_pixels(img, p1, p2, color);
	p1 = (t_point2){.x = WIDTH - 1, .y = HEIGHT / 2};
	p2 = (t_point2){.x = WIDTH / 2, .y = 0};
	connect_pixels(img, p1, p2, color);
	p1 = (t_point2){.x = WIDTH / 2, .y = 0};
	p2 = (t_point2){.x = WIDTH - 1, .y = HEIGHT / 2};
	connect_pixels(img, p1, p2, color);
	p1 = (t_point2){.x = WIDTH / 2, .y = HEIGHT - 1};
	p2 = (t_point2){.x = 0, .y = HEIGHT / 2};
	connect_pixels(img, p1, p2, color);
	p1 = (t_point2){.x = 0, .y = HEIGHT / 2};
	p2 = (t_point2){.x = WIDTH / 2, .y = HEIGHT - 1};
	connect_pixels(img, p1, p2, color);
	p1 = (t_point2){.x = WIDTH / 2, .y = HEIGHT - 1};
	p2 = (t_point2){.x = WIDTH - 1, .y = HEIGHT / 2};
	connect_pixels(img, p1, p2, color);
	p1 = (t_point2){.x = WIDTH - 1, .y = HEIGHT / 2};
	p2 = (t_point2){.x = WIDTH / 2, .y = HEIGHT - 1};
	connect_pixels(img, p1, p2, color);
}

void	fill_with_color(mlx_image_t *img, int color)
{
	int		w;
	int		h;
	size_t	i;

	w = img->width;
	h = img->height;
	i = 0;
	while (i < w * h)
	{
		mlx_put_pixel(img, i % w, (i / w) % h, color);
		i++;
	}
}

int	parse_landscape(t_matrix *m, const char *file_name)
{
	char		*line;
	int			fd;
	int			rows;
	int			cols;
	int			i;
	int			j;

	rows = count_rows(file_name);
	if (rows <= 0)
		return (FAIL);
	fd = open(file_name, O_RDONLY);
	if (fd < 0)
		return (FAIL);
	line = get_next_line(fd);
	if (!line)
		return (FAIL);
	cols = ft_strlen(line);
	if (ft_strchr(line, '\n'))
		cols--;
	m->elements = ft_calloc(rows, sizeof(int *));
	if (!m->elements)
		return (free_ptr_return_int((void *)&line, FAIL));
	i = -1;
	while (++i < rows)
	{
		m->elements[i] = ft_calloc(cols, sizeof(int));
		if (!m->elements[i])
			return (free_ptr_return_int((void *)&line, FAIL));
		j = -1;
		while (++j < cols && line)
		{
			m->elements[i][j] = ft_atoi(line);
			line = ft_strchr(line, ' ');
		}
		line = get_next_line(fd);
	}
	return (SUCCESS);
}

int	count_rows(const char *file_name)
{
	int	rows;
	int	fd;

	fd = open(file_name, O_RDONLY);
	if (fd < 0)
		return (-1);
	rows = 0;
	while (get_next_line(fd))
		rows++;
	close(fd);
	return (rows);
}

void	free_matrix_elements(t_matrix *m)
{
	if (m->rows)
		while (--m->rows)
			free(m->elements[m->rows]);
	free(m->elements);
	m->elements = NULL;
	m->cols = 0;
}

void	print_matrix(t_matrix *m)
{
	int	i;
	int	j;

	i = -1;
	while (++i < m->rows)
	{
		j = -1;
		while (++j < m->cols)
		{
			if (m->elements[i][j] > 0)
				ft_printf(" ");
			ft_printf("%d ", m->elements[i][j]);
		}
		ft_printf("\n");
	}
}
