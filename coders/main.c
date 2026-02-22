/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmena-li <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 11:52:13 by dmena-li          #+#    #+#             */
/*   Updated: 2026/02/19 13:34:00 by dmena-li         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	free_data(t_data *data)
{
	if (!data)
		return ;
	if (data->coders)
	{
		free(data->coders);
		data->coders = NULL;
	}
	if (data->dongles)
	{
		free(data->dongles);
		data->dongles = NULL;
	}
	free(data);
}

int	main(int argc, char **argv)
{
	t_data	*data;

	if (argc != 9)
		return (-1);
	data = parse(argv);
	if (!data)
		return (-1);
	virtual_data(data);
	free_data(data);
	return (0);
}
