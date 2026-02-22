/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmena-li <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 12:01:22 by dmena-li          #+#    #+#             */
/*   Updated: 2026/02/08 18:36:03 by dmena-li         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	check_type(char *av)
{
	if (av == NULL)
		return (0);
	if (ft_strcmp(av, "fifo") == 0)
		return (1);
	else if (ft_strcmp(av, "edf") == 0)
		return (2);
	return (0);
}

int	check_number(char *str)
{
	int	i;
	int	signs;

	i = 0;
	signs = 0;
	if (!str)
		return (0);
	while (str[i])
	{
		if (str[i] == '+' || str[i] == '-')
		{
			signs++;
			if (signs > 1 || str[i] == '-')
				return (0);
		}
		else if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

int	check_positiv(char **av)
{
	int	i;

	i = 1;
	while (i < 8)
	{
		if (!check_number(av[i]))
			return (0);
		i++;
	}
	return (1);
}

t_data	*parse(char **av)
{
	int		i;
	long	x[8];

	if (!check_positiv(av))
		return (NULL);
	i = 0;
	while (i < 7)
	{
		x[i] = ft_atol(av[i + 1]);
		if (x[i] < 0)
			return (NULL);
		i++;
	}
	x[i] = check_type(av[i + 1]);
	if (x[i] == 0)
		return (NULL);
	return (data_create(x));
}
