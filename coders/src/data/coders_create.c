/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coders_create.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmena-li <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/12 15:04:29 by dmena-li          #+#    #+#             */
/*   Updated: 2026/02/22 13:22:49 by dmena-li         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	create_coder(t_data *data, t_coder *coder, int i)
{
	coder->id = i;
	coder->compiles_done = 0;
	coder->last_compile_start = 0;
	coder->data = data;
	pthread_mutex_init(&coder->compile_mutex, NULL);
	if (i == 0)
		coder->l_dg = &data->dongles[data->number_of_coders - 1];
	else
		coder->l_dg = &data->dongles[i - 1];
	coder->r_dg = &data->dongles[i];
}

t_coder	*virtual_coders(t_data *data)
{
	int		i;
	t_coder	*coders;

	i = 0;
	coders = malloc(sizeof(t_coder) * data->number_of_coders);
	if (!coders)
		return (NULL);
	data->stop_sim = 0;
	pthread_mutex_init(&data->write_lock, NULL);
	pthread_mutex_init(&data->death_lock, NULL);
	while (i < data->number_of_coders)
	{
		create_coder(data, &coders[i], i);
		i++;
	}
	return (coders);
}
