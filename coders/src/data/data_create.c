/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data_create.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmena-li <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/08 16:39:41 by dmena-li          #+#    #+#             */
/*   Updated: 2026/02/18 14:39:30 by dmena-li         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

t_data	*data_create(long *tokens)
{
	t_data	*data;

	if (tokens[0] < 1)
		return (0);
	data = (t_data *)malloc(sizeof(t_data));
	if (!data)
		return (NULL);
	data->number_of_coders = tokens[0];
	data->time_to_burnout = tokens[1];
	data->time_to_compile = tokens[2];
	data->time_to_debug = tokens[3];
	data->time_to_refactor = tokens[4];
	data->number_of_compiles_required = tokens[5];
	data->dongle_cooldown = tokens[6];
	data->scheduler = tokens[7];
	data->dongles = dongles_array(tokens[0], tokens[6]);
	if (!data->dongles)
		return (free(data), NULL);
	return (data);
}
