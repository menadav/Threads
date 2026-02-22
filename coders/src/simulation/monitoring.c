/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitoring.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmena-li <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/12 12:25:51 by dmena-li          #+#    #+#             */
/*   Updated: 2026/02/20 11:46:03 by dmena-li         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	compiles_end(t_data *data, long end)
{
	if (end >= data->number_of_coders)
	{
		pthread_mutex_lock(&data->death_lock);
		data->stop_sim = 1;
		pthread_mutex_unlock(&data->death_lock);
		return (1);
	}
	return (0);
}

int	monitor_burnout(t_data *data)
{
	long	i;
	long	end;

	i = 0;
	end = 0;
	while (i < data->number_of_coders)
	{
		pthread_mutex_lock(&data->coders[i].compile_mutex);
		if (get_time_in_ms() - data->coders[i].last_compile_start
			>= data->time_to_burnout)
		{
			pthread_mutex_lock(&data->death_lock);
			data->stop_sim = 1;
			pthread_mutex_unlock(&data->death_lock);
			write_status(&data->coders[i], "burned out");
			pthread_mutex_unlock(&data->coders[i].compile_mutex);
			return (1);
		}
		if (data->coders[i].compiles_done >= data->number_of_compiles_required)
			end++;
		pthread_mutex_unlock(&data->coders[i].compile_mutex);
		i++;
	}
	return (compiles_end(data, end));
}

void	*monitor_routine(void *arg)
{
	t_data	*data;

	data = (t_data *)arg;
	while (1)
	{
		if (monitor_burnout(data))
			return (NULL);
		usleep(2000);
	}
	return (NULL);
}
