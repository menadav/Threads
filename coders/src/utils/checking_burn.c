/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checking_burn.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmena-li <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/14 12:30:25 by dmena-li          #+#    #+#             */
/*   Updated: 2026/02/18 19:36:19 by dmena-li         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	checking_bornout(t_coder *coder)
{
	int	stop;

	pthread_mutex_lock(&coder->data->death_lock);
	stop = coder->data->stop_sim;
	pthread_mutex_unlock(&coder->data->death_lock);
	return (stop);
}
