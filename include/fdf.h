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

# define WIDTH	2048
# define HEIGHT	2048

# define RED	0xFF0000FF
# define GREEN	0x00FF00FF
# define BLUE	0x0000FFFF
# define WHITE	0xFFFFFFFF
# define BLACK	0x000000FF

# define SUCCESS	0
# define FAILURE	-1

typedef struct s_dp3
{
	double	x;
	double	y;
	double	z;
}	t_dp3;

typedef struct s_ip2
{
	int	x;
	int	y;
}	t_ip2;

typedef struct s_dp2
{
	double	x;
	double	y;
}	t_dp2;

typedef struct s_dmatrix
{
	int		rows;
	int		cols;
	double	**elem;
}	t_dmatrix;

typedef struct s_linvars
{
	int		delta_x;
	int		delta_y;
	double	slope;
	int		step_x;
	int		step_y;
}	t_linvars;

typedef struct s_fdf_data
{
	mlx_t			*mlx;
	mlx_image_t		*img;
	int				fd;
	int				rows;
	int				cols;
	int				**map;
	unsigned int	**colors;
	t_dp3			**world;
	t_dp2			**view;
	t_ip2			**pixels;
	double			view_scale;
	t_dp2			view_offset;
}	t_fdf_data;

void	free_data(t_fdf_data *data);
void	free_close_print_exit(t_fdf_data *data, const char *error);

void	print_map(t_fdf_data *data);
void	print_colors(t_fdf_data *data);

/*----------------------------------------------------------------------------*/
/*                             map_handling.c                                 */
/*----------------------------------------------------------------------------*/
int		validate_map_file(const char *file_name, t_fdf_data *data);
void	parse_map_file(const char *file_name, t_fdf_data *data);
int		count_rows(const char *file_name);
/*----------------------------------------------------------------------------*/
int	create_points(t_fdf_data *data);
/*----------------------------------------------------------------------------*/
#endif
