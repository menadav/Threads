/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_create.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmena-li <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/12 15:08:16 by dmena-li          #+#    #+#             */
/*   Updated: 2026/02/22 13:27:52 by dmena-li         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	create_dongle(t_dongle *dongles, long max, long cldw)
{
	long	i;

	i = 0;
	while (i < max)
	{
		if (pthread_mutex_init(&dongles[i].dongl, NULL) != 0)
			return (1);
		if (pthread_mutex_init(&dongles[i].queue_lock, NULL) != 0)
			return (1);
		if (pthread_mutex_init(&dongles[i].cldw_lock, NULL) != 0)
			return (1);
		dongles[i].available_cldw = 0;
		dongles[i].is_taken = 0;
		dongles[i].cldw = cldw;
		dongles[i].second_queue = NULL;
		dongles[i].first_queue = NULL;
		i++;
	}
	return (0);
}

t_dongle	*dongles_array(long max, long cldw)
{
	t_dongle	*dongles;

	dongles = (t_dongle *)malloc(sizeof(t_dongle) * max);
	if (!dongles)
		return (NULL);
	if (create_dongle(dongles, max, cldw) != 0)
		return (free(dongles), NULL);
	return (dongles);
}
