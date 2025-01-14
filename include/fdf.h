/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvarila <jvarila@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 10:20:39 by jvarila           #+#    #+#             */
/*   Updated: 2025/01/13 17:26:46 by jvarila          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FDF_H
# define FDF_H

# include "../lib/libft/libft.h"
# include "../lib/MLX42/include/MLX42/MLX42.h"
# include <math.h>
# include <fcntl.h>

# define WIDTH 1024
# define HEIGHT 1024

# define RED	0xFF0000FF
# define GREEN	0x00FF00FF
# define BLUE	0x0000FFFF
# define WHITE	0xFFFFFFFF
# define BLACK	0x000000FF

# define SUCCESS	0
# define FAIL		-1

typedef struct s_point3
{
	double	x;
	double	y;
	double	z;
}	t_point3;

typedef struct s_point2
{
	int	x;
	int	y;
}	t_point2;

typedef struct s_matrix
{
	int	rows;
	int	cols;
	int	**elements;
}	t_matrix;

void	connect_points(mlx_image_t *img, t_point2 p1, t_point2 p2);

#endif
