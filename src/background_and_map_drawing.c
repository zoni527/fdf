/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   background_and_map_drawing.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvarila <jvarila@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 13:33:30 by jvarila           #+#    #+#             */
/*   Updated: 2025/01/23 13:34:37 by jvarila          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/fdf.h"

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

void	print_map(t_fdf *data)
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

void	print_colors(t_fdf *data)
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

void	redraw(t_fdf *data)
{
	fill_with_color(data->img, BG_COLOR);
	view_to_pixels(data);
	draw_map(data);
}
