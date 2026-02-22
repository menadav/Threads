/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   virtual.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmena-li <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/10 10:22:33 by dmena-li          #+#    #+#             */
/*   Updated: 2026/02/22 13:33:42 by dmena-li         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	*create_threads(t_coder *coders, t_data *data)
{
	pthread_t	monitor;
	int			i;

	i = 0;
	data->start_time = get_time_in_ms();
	while (i < data->number_of_coders)
	{
		coders[i].last_compile_start = data->start_time;
		pthread_create(&(coders[i].thread_id), NULL,
			&coder_routine, &coders[i]);
		i++;
	}
	pthread_create(&monitor, NULL, &monitor_routine, data);
	i = 0;
	while (i < data->number_of_coders)
	{
		pthread_join(coders[i].thread_id, NULL);
		i++;
	}
	pthread_join(monitor, NULL);
	return (NULL);
}

static void	destroy_all_mutex(t_data *data)
{
	int	i;

	if (!data || !data->coders || !data->dongles)
		return ;
	i = 0;
	while (i < data->number_of_coders)
	{
		pthread_mutex_destroy(&data->coders[i].compile_mutex);
		pthread_mutex_destroy(&data->dongles[i].dongl);
		pthread_mutex_destroy(&data->dongles[i].cldw_lock);
		pthread_mutex_destroy(&data->dongles[i].queue_lock);
		i++;
	}
	pthread_mutex_destroy(&data->death_lock);
	pthread_mutex_destroy(&data->write_lock);
}

void	virtual_data(t_data *data)
{
	data->coders = virtual_coders(data);
	create_threads(data->coders, data);
	destroy_all_mutex(data);
}
