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

# define WIDTH	1024
# define HEIGHT	1024

# define RED	0xFF0000FF
# define GREEN	0x00FF00FF
# define BLUE	0x0000FFFF
# define WHITE	0xFFFFFFFF
# define BLACK	0x000000FF

# define SUCCESS	0
# define FAIL		-1

typedef struct s_p3
{
	double	x;
	double	y;
	double	z;
}	t_p3;

typedef struct s_p2
{
	int	x;
	int	y;
}	t_p2;

typedef struct s_matrix
{
	int	rows;
	int	cols;
	int	**elem;
}	t_matrix;

typedef struct s_linvars
{
	int		delta_x;
	int		delta_y;
	double	slope;
	int		step_x;
	int		step_y;
}	t_linvars;

typedef struct s_p3arr
{
	int			rows;
	int			cols;
	t_p3	**points;
}	t_p3arr;

/*----------------------------------------------------------------------------*/
/*                             map_handling.c                                 */
/*----------------------------------------------------------------------------*/
int		validate_map_file(const char *file_name);
int		parse_map_file(const char *file_name, t_matrix *m);
int		count_rows(const char *file_name);
/*----------------------------------------------------------------------------*/
/*                                matrix.c                                    */
/*----------------------------------------------------------------------------*/
int		create_matrix_elements(t_matrix *m);
void	fill_matrix_row(t_matrix *m, int row, char *line);
void	free_matrix_elements(t_matrix *m);
void	print_matrix(t_matrix *m);
/*----------------------------------------------------------------------------*/
int	create_points(t_p3arr *points, t_matrix *matrix);
/*----------------------------------------------------------------------------*/
#endif
