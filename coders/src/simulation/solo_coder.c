/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   solo_coder.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmena-li <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/18 19:05:05 by dmena-li          #+#    #+#             */
/*   Updated: 2026/02/20 11:45:24 by dmena-li         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	solo_coder(t_coder *coder)
{
	pthread_mutex_lock(&coder->r_dg->dongl);
	write_status(coder, "has taken a dongle");
	usleep((coder->data->time_to_burnout * 1000) + 1000);
	pthread_mutex_unlock(&coder->r_dg->dongl);
}
