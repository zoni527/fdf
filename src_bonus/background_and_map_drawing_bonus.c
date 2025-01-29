/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   background_and_map_drawing_bonus.c                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvarila <jvarila@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 13:33:30 by jvarila           #+#    #+#             */
/*   Updated: 2025/01/29 11:13:44 by jvarila          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/fdf.h"

static int	on_screen(const t_pixel *p1, const t_pixel *p2);

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

void	draw_map(t_fdf *data)
{
	int	i;
	int	j;

	i = -1;
	while (++i < data->rows - 1)
	{
		j = -1;
		while (++j < data->cols - 1)
		{
			if (on_screen(&data->pixels[i][j], &data->pixels[i + 1][j]))
				draw_segment(data, data->pixels[i][j], data->pixels[i + 1][j]);
			if (on_screen(&data->pixels[i][j], &data->pixels[i][j + 1]))
				draw_segment(data, data->pixels[i][j], data->pixels[i][j + 1]);
		}
	}
	j = data->cols - 1;
	i = -1;
	while (++i < data->rows - 1)
		if (on_screen(&data->pixels[i][j], &data->pixels[i + 1][j]))
			draw_segment(data, data->pixels[i][j], data->pixels[i + 1][j]);
	i = data->rows - 1;
	j = -1;
	while (++j < data->cols - 1)
		if (on_screen(&data->pixels[i][j], &data->pixels[i][j + 1]))
			draw_segment(data, data->pixels[i][j], data->pixels[i][j + 1]);
}

static int	on_screen(const t_pixel *p1, const t_pixel *p2)
{
	if ((p1->x < 0 && p2->x < 0) || (p1->x > WIDTH && p2->x > WIDTH)
		|| (p1->y < 0 && p2->y < 0) || (p1->y > HEIGHT && p2->y > HEIGHT))
		return (0);
	return (1);
}

void	redraw(t_fdf *data)
{
	fill_with_color(data->img, BG_COLOR);
	view_to_pixels(data);
	draw_map(data);
}
