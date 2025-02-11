/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvarila <jvarila@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 17:13:20 by jvarila           #+#    #+#             */
/*   Updated: 2025/01/27 16:50:01 by jvarila          ###   ########.fr       */
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

# define RED		0xff0000ff
# define GREEN		0x00ff00ff
# define BLUE		0x0000ffff
# define WHITE		0xffffffff
# define BLACK		0x000000ff
# define BG_COLOR	0x111111ff

# define LINE_GRADIENT_COLOR_1	0xffffffff
# define LINE_GRADIENT_COLOR_2	0x44aaffff

# define SUCCESS	0
# define FAILURE	-1

# define ONE_DEGREE	0.01745329252

typedef struct s_dp3
{
	double	x;
	double	y;
	double	z;
}	t_dp3;

typedef struct s_dp2
{
	double	x;
	double	y;
}	t_dp2;

typedef struct s_pixel
{
	int				x;
	int				y;
	unsigned int	rgba;
}	t_pixel;

typedef struct s_linvars
{
	int		delta_x;
	int		delta_y;
	double	slope;
	int		step_x;
	int		step_y;
}	t_linvars;

typedef struct s_fdf
{
	mlx_t			*mlx;
	mlx_image_t		*img;
	int				fd;
	int				rows;
	int				cols;
	int				**map;
	t_dp3			**world;
	t_dp2			**view;
	t_pixel			**pixels;
	double			view_scale;
	t_dp2			view_offset;
	void			(*project)(struct s_fdf *data);
}	t_fdf;

typedef struct s_color
{
	unsigned int	rgba;
	unsigned char	r;
	unsigned char	g;
	unsigned char	b;
	unsigned char	a;
}	t_color;

/* fdf.c ---------------------------------------------------------------------*/
void		set_up_scene(t_fdf *data);
void		free_close_print_exit(t_fdf *data, const char *error);
void		allocate_data(t_fdf *data);
void		free_data(t_fdf *data);
/* map_handling.c ------------------------------------------------------------*/
int			validate_map_file(const char *file_name, t_fdf *data);
void		parse_map_file(const char *file_name, t_fdf *data);
/* world_manipulation.c ------------------------------------------------------*/
void		assign_world(t_fdf *data);
void		offset_world(t_fdf *data, t_dp3 offset);
void		scale_world(t_fdf *data, double multiplier);
void		rotate_world(t_fdf *data, double rx, double ry, double rz);
/* world_centering.c ---------------------------------------------------------*/
void		center_world(t_fdf *data);
void		center_world_x(t_fdf *data);
void		center_world_y(t_fdf *data);
void		center_world_z(t_fdf *data);
/* world_min.c ---------------------------------------------------------------*/
double		min_x_world(t_fdf *data);
double		min_y_world(t_fdf *data);
double		min_z_world(t_fdf *data);
/* world_max.c ---------------------------------------------------------------*/
double		max_x_world(t_fdf *data);
double		max_y_world(t_fdf *data);
double		max_z_world(t_fdf *data);
/* 3d_point_manipulation.c ---------------------------------------------------*/
void		offset_dp3(t_dp3 *point, t_dp3 offset);
void		scale_dp3(t_dp3 *point, double multiplier);
void		rotate_dp3(t_dp3 *point, double rx, double ry, double rz);
void		matrix_transform(t_dp3 *p, double **t);
/* 2d_point_manipulation.c ---------------------------------------------------*/
void		offset_dp2(t_dp2 *point, t_dp2 offset);
void		scale_dp2(t_dp2 *point, double multiplier);
void		rotate_dp2(t_dp2 *point, double angle);
/* view_projection.c ---------------------------------------------------------*/
void		y_plane_projection(t_fdf *data);
void		x_plane_projection(t_fdf *data);
void		z_plane_projection(t_fdf *data);
void		oblique_projection(t_fdf *data);
/* view_manipulation.c -------------------------------------------------------*/
void		offset_view(t_fdf *data, t_dp2 offset);
void		scale_view(t_fdf *data, double multiplier);
void		rotate_view(t_fdf *data, double angle);
void		stretch_view_to_window_scale(t_fdf *data);
/* view_centering.c ----------------------------------------------------------*/
void		center_view(t_fdf *data);
void		center_view_x(t_fdf *data);
void		center_view_y(t_fdf *data);
/* view_min_max.c ------------------------------------------------------------*/
double		min_x_view(t_fdf *data);
double		min_y_view(t_fdf *data);
double		max_x_view(t_fdf *data);
double		max_y_view(t_fdf *data);
/* pixels_and_colors.c -------------------------------------------------------*/
void		initialize_pixels(t_fdf *data);
void		view_to_pixels(t_fdf *data);
int			interpolate_color_channels(t_pixel p1, t_pixel p2, double d, int i);
int			interpolate_channel(int p1_c, int p2_c, double distance, int index);
/* background_and_map_drawing.c ----------------------------------------------*/
void		fill_with_color(mlx_image_t *img, unsigned int rgba);
void		draw_map(t_fdf *data);
void		redraw(t_fdf *data);
/* default_gradient.c --------------------------------------------------------*/
void		set_default_gradient(t_fdf *data);
void		interpolate_default_gradient(t_fdf *data, t_color c1, t_dp3 k);
int			no_color_info(t_fdf *data);
/* line_segment_drawing.c ----------------------------------------------------*/
void		draw_segment(t_fdf *data, t_pixel p1, t_pixel p2);
/* input_01.c ----------------------------------------------------------------*/
void		hook(void *param);
void		esc_hook(void *param);
/* input_02.c ----------------------------------------------------------------*/
void		scroll_hook(double xdelta, double ydelta, void *param);
void		projection_hook(void *param);
void		scale_z_hook(void *param);
void		reset_hook(void *param);
/* non_uniform_scaling.c -----------------------------------------------------*/
void		scale_world_x(t_fdf *data, double multiplier);
void		scale_world_y(t_fdf *data, double multiplier);
void		scale_world_z(t_fdf *data, double multiplier);
/* data_printing.c -----------------------------------------------------------*/
void		print_map(const t_fdf *data);
void		print_colors(const t_fdf *data);
void		print_controls(void);

#endif
