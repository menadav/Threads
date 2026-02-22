/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   edf_compile.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmena-li <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 17:19:34 by dmena-li          #+#    #+#             */
/*   Updated: 2026/02/20 11:47:52 by dmena-li         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	exit_queue(t_coder *prio, t_dongle *dongle)
{
	if (prio == dongle->second_queue)
		dongle->second_queue = NULL;
	else
	{
		dongle->first_queue = dongle->second_queue;
		dongle->second_queue = NULL;
	}
}

static t_coder	*prio_edf(t_dongle *dongle)
{
	t_coder		*c1;
	t_coder		*c2;
	long		deadline1;
	long		deadline2;

	pthread_mutex_lock(&dongle->queue_lock);
	c1 = dongle->first_queue;
	c2 = dongle->second_queue;
	pthread_mutex_unlock(&dongle->queue_lock);
	pthread_mutex_lock(&dongle->first_queue->compile_mutex);
	deadline1 = c1->last_compile_start;
	pthread_mutex_unlock(&dongle->first_queue->compile_mutex);
	pthread_mutex_lock(&dongle->second_queue->compile_mutex);
	deadline2 = c2->last_compile_start;
	pthread_mutex_unlock(&dongle->second_queue->compile_mutex);
	if (deadline1 < deadline2)
		return (c1);
	return (c2);
}

static t_coder	*edf_dongle(t_coder *coder, t_dongle *dongle)
{
	pthread_mutex_lock(&dongle->queue_lock);
	if (dongle->first_queue == NULL && dongle->second_queue == NULL)
		dongle->first_queue = coder;
	else if (dongle->first_queue != NULL && dongle->second_queue == NULL)
	{
		dongle->second_queue = coder;
		return (prio_edf(dongle));
	}
	return (dongle->first_queue);
}

static int	wait_for_dongle(t_coder *coder, t_dongle *dongle)
{
	t_coder	*prio;

	while (1)
	{
		if (checking_bornout(coder))
			return (1);
		if (check_take_dongle(dongle, coder))
		{
			prio = edf_dongle(coder, dongle);
			if (prio == coder)
			{
				dongle->is_taken = 1;
				pthread_mutex_unlock(&dongle->cldw_lock);
				exit_queue(prio, dongle);
				pthread_mutex_unlock(&dongle->queue_lock);
				pthread_mutex_lock(&dongle->dongl);
				write_status(coder, "has taken a dongle");
				return (0);
			}
			pthread_mutex_unlock(&dongle->cldw_lock);
			pthread_mutex_unlock(&dongle->queue_lock);
		}
		pthread_mutex_unlock(&dongle->cldw_lock);
		usleep(100);
	}
}

int	edf_compile(t_coder *coder)
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
