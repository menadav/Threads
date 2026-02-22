/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmena-li <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/14 17:01:18 by dmena-li          #+#    #+#             */
/*   Updated: 2026/02/22 13:04:12 by dmena-li         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	compile_action(t_coder *coder)
{
	if (coder->data->scheduler == 1)
	{
		if (fifo_compile(coder) != 0)
			return ;
	}
	if (coder->data->scheduler == 2)
	{
		if (edf_compile(coder))
			return ;
	}
	write_status(coder, "is compiling");
	last_compile(coder);
	take_dongle(coder->r_dg);
	take_dongle(coder->l_dg);
	pthread_mutex_unlock(&coder->r_dg->dongl);
	pthread_mutex_unlock(&coder->l_dg->dongl);
}

void	debug_action(t_coder *coder)
{
	write_status(coder, "is debugging");
	usleep(coder->data->time_to_debug * 1000);
}

void	refactor_action(t_coder *coder)
{
	write_status(coder, "is refactoring");
	usleep(coder->data->time_to_refactor * 1000);
}

void	*coder_routine(void *arg)
{
	t_coder	*coder;

	coder = (t_coder *)arg;
	if (coder->data->number_of_coders == 1)
		return (solo_coder(coder), NULL);
	while (1)
	{
		if (checking_bornout(coder))
			break ;
		compile_action(coder);
		debug_action(coder);
		refactor_action(coder);
		pthread_mutex_lock(&coder->compile_mutex);
		if (coder->compiles_done == coder->data->number_of_compiles_required)
		{
			pthread_mutex_unlock(&coder->compile_mutex);
			break ;
		}
		pthread_mutex_unlock(&coder->compile_mutex);
	}
	return (NULL);
}
