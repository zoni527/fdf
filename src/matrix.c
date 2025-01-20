/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matrix.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvarila <jvarila@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 13:36:36 by jvarila           #+#    #+#             */
/*   Updated: 2025/01/15 13:52:39 by jvarila          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/fdf.h"

int	create_matrix_elements(t_imatrix *m)
{
	int	i;

	m->elem = ft_calloc(m->rows, sizeof(int *));
	if (!m->elem)
		return (FAILURE);
	i = -1;
	while (++i < m->rows)
	{
		m->elem[i] = ft_calloc(m->cols, sizeof(int));
		if (!m->elem[i])
		{
			free_matrix_elements(m);
			return (FAILURE);
		}
	}
	return (SUCCESS);
}

void	fill_matrix_row(t_imatrix *m, int row, const char *line)
{
	int		i;

	i = -1;
	while (++i < m->cols)
	{
		m->elem[row][i] = ft_atoi(line);
		line = ft_strchr(skip_whitespace(line), ' ');
	}
}

void	free_matrix_elements(t_imatrix *m)
{
	if (m->rows)
		while (m->rows--)
			free(m->elem[m->rows]);
	free(m->elem);
	m->elem = NULL;
	m->cols = 0;
	m->rows = 0;
}

void	print_matrix(const t_imatrix *m)
{
	int	i;
	int	j;

	i = -1;
	while (++i < m->rows)
	{
		j = -1;
		while (++j < m->cols)
			ft_printf("%d ", m->elem[i][j]);
		ft_printf("\n");
	}
}
