/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hex_tests.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvarila <jvarila@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 14:07:46 by jvarila           #+#    #+#             */
/*   Updated: 2025/01/20 14:11:01 by jvarila          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/fdf.h"

int	main(int argc, char *argv[])
{
	if (valid_hex_string(argv[1]))
		ft_printf("%s is a valid hex string\n", argv[1]);
	else
		ft_printf("%s is invalid\n", argv[1]);
	return (0);
}
