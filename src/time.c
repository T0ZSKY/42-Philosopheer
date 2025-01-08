/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomlimon <tom.limon@>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 16:47:38 by tomlimon          #+#    #+#             */
/*   Updated: 2025/01/08 16:48:36 by tomlimon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/philo.h"

void ft_smart_sleep(long long time, t_table *table)
{
    long long start;
    long long now;

    start = get_timestamp(table);
    while (1)
    {
        pthread_mutex_lock(&table->status_mutex);
        if (!table->simulation_running)
        {
            pthread_mutex_unlock(&table->status_mutex);
            break;
        }
        pthread_mutex_unlock(&table->status_mutex);

        now = get_timestamp(table);
        if ((now - start) >= time)
            break;
        usleep(500);
    }
}

long long ft_get_time(void)
{
    struct timeval tv;
    long long milliseconds;

    gettimeofday(&tv, NULL);
    milliseconds = (tv.tv_sec * 1000LL) + (tv.tv_usec / 1000);
    return milliseconds;
}
long long get_timestamp(t_table *table)
{
    struct timeval current_time;
    gettimeofday(&current_time, NULL);
    return ((current_time.tv_sec * 1000 + current_time.tv_usec / 1000) - table->start_time);
}
