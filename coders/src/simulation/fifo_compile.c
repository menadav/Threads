/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fifo_compile.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmena-li <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/16 20:54:33 by dmena-li          #+#    #+#             */
/*   Updated: 2026/02/20 11:47:30 by dmena-li         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	fifo_dongle(t_coder *coder, t_dongle *dongle)
{
	pthread_mutex_lock(&dongle->queue_lock);
	if (dongle->first_queue == NULL && dongle->second_queue == NULL)
		dongle->first_queue = coder;
	else if (dongle->first_queue != NULL && dongle->second_queue == NULL)
		dongle->second_queue = coder;
	pthread_mutex_unlock(&dongle->queue_lock);
	return (0);
}

static int	wait_for_dongle(t_coder *coder, t_dongle *dongle)
{
	if (fifo_dongle(coder, dongle))
		return (1);
	while (1)
	{
		if (checking_bornout(coder))
			return (1);
		pthread_mutex_lock(&dongle->queue_lock);
		if (dongle->first_queue == coder)
		{
			if (can_take_dongle(dongle, coder))
			{
				pthread_mutex_lock(&dongle->dongl);
				dongle->first_queue = dongle->second_queue;
				dongle->second_queue = NULL;
				write_status(coder, "has taken a dongle");
				pthread_mutex_unlock(&dongle->queue_lock);
				return (0);
			}
		}
		pthread_mutex_unlock(&dongle->queue_lock);
		usleep(500);
	}
}

int	fifo_compile(t_coder *coder)
{
	if ((coder->id % 2) == 0)
	{
		if (wait_for_dongle(coder, coder->r_dg))
			return (1);
		if (wait_for_dongle(coder, coder->l_dg))
		{
			pthread_mutex_unlock(&coder->r_dg->dongl);
			return (1);
		}
	}
	else
	{
		if (wait_for_dongle(coder, coder->l_dg))
			return (1);
		if (wait_for_dongle(coder, coder->r_dg))
		{
			pthread_mutex_unlock(&coder->l_dg->dongl);
			return (1);
		}
	}
	return (0);
}
