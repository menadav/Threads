/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmena-li <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 15:23:09 by dmena-li          #+#    #+#             */
/*   Updated: 2026/02/20 11:49:10 by dmena-li         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	last_compile(t_coder *coder)
{
	usleep(coder->data->time_to_compile * 1000);
	pthread_mutex_lock(&coder->compile_mutex);
	coder->last_compile_start = get_time_in_ms();
	pthread_mutex_unlock(&coder->compile_mutex);
	pthread_mutex_lock(&coder->compile_mutex);
	coder->compiles_done++;
	pthread_mutex_unlock(&coder->compile_mutex);
}

void	free_queue(t_dongle *dongle1, t_dongle *dongle2)
{
	pthread_mutex_lock(&dongle1->queue_lock);
	dongle1->first_queue = dongle1->second_queue;
	dongle1->second_queue = NULL;
	pthread_mutex_unlock(&dongle1->queue_lock);
	pthread_mutex_lock(&dongle2->queue_lock);
	dongle2->first_queue = dongle2->second_queue;
	dongle2->second_queue = NULL;
	pthread_mutex_unlock(&dongle2->queue_lock);
}
