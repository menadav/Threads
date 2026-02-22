/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   write_status.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmena-li <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/12 15:21:54 by dmena-li          #+#    #+#             */
/*   Updated: 2026/02/18 18:47:20 by dmena-li         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	write_status(t_coder *coder, char *status)
{
	long	time;

	pthread_mutex_lock(&coder->data->death_lock);
	time = get_time_in_ms() - coder->data->start_time;
	if (coder->data->stop_sim)
	{
		if (ft_strcmp(status, "burned out") == 0
			|| ft_strcmp(status, "died") == 0)
		{
			pthread_mutex_lock(&coder->data->write_lock);
			printf("%ld %d %s\n", time, coder->id, status);
			pthread_mutex_unlock(&coder->data->write_lock);
		}
		pthread_mutex_unlock(&coder->data->death_lock);
		return (-1);
	}
	pthread_mutex_lock(&coder->data->write_lock);
	printf("%ld %d %s\n", time, coder->id, status);
	pthread_mutex_unlock(&coder->data->write_lock);
	pthread_mutex_unlock(&coder->data->death_lock);
	return (0);
}
