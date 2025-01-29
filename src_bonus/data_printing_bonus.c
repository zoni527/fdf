/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data_printing_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvarila <jvarila@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 14:30:44 by jvarila           #+#    #+#             */
/*   Updated: 2025/01/29 11:13:26 by jvarila          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/fdf.h"

void	print_map(const t_fdf *data)
{
	int	i;
	int	j;

	i = -1;
	while (++i < data->rows)
	{
		j = -1;
		while (++j < data->cols)
			ft_printf("%d ", data->map[i][j]);
		ft_printf("\n");
	}
}

void	print_colors(const t_fdf *data)
{
	int	i;
	int	j;

	i = -1;
	while (++i < data->rows)
	{
		j = -1;
		while (++j < data->cols)
			ft_printf("0x%x ", data->pixels[i][j].rgba);
		ft_printf("\n");
	}
}
