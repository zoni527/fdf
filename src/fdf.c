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

void	connect_pixels(mlx_image_t *img, t_point2 p1, t_point2 p2);

int	main(int argc, char *argv[])
{
	mlx_t		*mlx;
	mlx_image_t	*img;
	t_point2	p1;
	t_point2	p2;
	int			i;

	mlx = mlx_init(WIDTH, HEIGHT, "fdf", 1);
	if (!mlx)
		return (1);
	img = mlx_new_image(mlx, WIDTH, HEIGHT);
	if (!img || mlx_image_to_window(mlx, img, 0, 0) < 0)
	{
		free(mlx);
		return (1);
	}
	i = -1;
	while (++i < WIDTH * HEIGHT)
		mlx_put_pixel(img, i % WIDTH, (i / WIDTH) % HEIGHT, BLACK);
	p1 = (t_point2){.x = 0, .y = 0};
	p2 = (t_point2){.x = 0, .y = 0};
	connect_pixels(img, p1, p2);
	p1 = (t_point2){.x = WIDTH - 1, .y = HEIGHT - 1};
	p2 = (t_point2){.x = 0, .y = 0};
	connect_pixels(img, p1, p2);
	p1 = (t_point2){.x = 0, .y = 0};
	p2 = (t_point2){.x = WIDTH - 1, .y = HEIGHT - 1};
	connect_pixels(img, p1, p2);
	p1 = (t_point2){.x = 0, .y = HEIGHT - 1};
	p2 = (t_point2){.x = WIDTH - 1, .y = 0};
	connect_pixels(img, p1, p2);
	p1 = (t_point2){.x = WIDTH - 1, .y = 0};
	p2 = (t_point2){.x = 0, .y = HEIGHT - 1};
	connect_pixels(img, p1, p2);
	p1 = (t_point2){.x = WIDTH/2, .y = 0};
	p2 = (t_point2){.x = WIDTH/2, .y = HEIGHT - 1};
	connect_pixels(img, p1, p2);
	p1 = (t_point2){.x = WIDTH/2, .y = HEIGHT - 1};
	p2 = (t_point2){.x = WIDTH/2, .y = 0};
	connect_pixels(img, p1, p2);
	p1 = (t_point2){.x = 0, .y = HEIGHT / 2};
	p2 = (t_point2){.x = WIDTH - 1, .y = HEIGHT / 2};
	connect_pixels(img, p1, p2);
	p1 = (t_point2){.x = WIDTH - 1, .y = HEIGHT / 2};
	p2 = (t_point2){.x = 0, .y = HEIGHT / 2};
	connect_pixels(img, p1, p2);
	p1 = (t_point2){.x = WIDTH / 2, .y = 0};
	p2 = (t_point2){.x = 0, .y = HEIGHT / 2};
	connect_pixels(img, p1, p2);
	p1 = (t_point2){.x = 0, .y = HEIGHT / 2};
	p2 = (t_point2){.x = WIDTH / 2, .y = 0};
	connect_pixels(img, p1, p2);
	p1 = (t_point2){.x = WIDTH - 1, .y = HEIGHT / 2};
	p2 = (t_point2){.x = WIDTH / 2, .y = 0};
	connect_pixels(img, p1, p2);
	p1 = (t_point2){.x = WIDTH / 2, .y = 0};
	p2 = (t_point2){.x = WIDTH - 1, .y = HEIGHT / 2};
	connect_pixels(img, p1, p2);
	p1 = (t_point2){.x = WIDTH / 2, .y = HEIGHT - 1};
	p2 = (t_point2){.x = 0, .y = HEIGHT / 2};
	connect_pixels(img, p1, p2);
	p1 = (t_point2){.x = 0, .y = HEIGHT / 2};
	p2 = (t_point2){.x = WIDTH / 2, .y = HEIGHT - 1};
	connect_pixels(img, p1, p2);
	p1 = (t_point2){.x = WIDTH / 2, .y = HEIGHT - 1};
	p2 = (t_point2){.x = WIDTH - 1, .y = HEIGHT / 2};
	connect_pixels(img, p1, p2);
	p1 = (t_point2){.x = WIDTH - 1, .y = HEIGHT / 2};
	p2 = (t_point2){.x = WIDTH / 2, .y = HEIGHT - 1};
	connect_pixels(img, p1, p2);
	mlx_loop(mlx);
	mlx_terminate(mlx);
	return (0);
}

void	connect_pixels(mlx_image_t *img, t_point2 p1, t_point2 p2)
{
	int		delta_x;
	int		delta_y;
	double	k;
	int	sx;
	int	sy;
	int	i;

	if (p1.x < 0 || p1.y < 0 || p2.x < 0 || p2.y < 0
		|| p1.x >= WIDTH || p1.y >= HEIGHT || p2.x >= WIDTH || p2.y >= HEIGHT)
		return ;
	delta_x = p2.x - p1.x;
	delta_y = p2.y - p1.y;
	sx = 1 + (delta_x < 0) * (-2);
	sy = 1 + (delta_y < 0) * (-2);
	i = -1;
	if (delta_x == 0)
	{
		while (++i < abs(delta_y) + 1)
			mlx_put_pixel(img, p1.x, (int)(p1.y + i * sy), BLUE);
		return ;
	}
	k = fabs((double)delta_y / delta_x);
	while (++i < abs(delta_x) + 1)
		mlx_put_pixel(img, p1.x + (i * sx), (int)(p1.y + (i * sy) * k), BLUE);
}
