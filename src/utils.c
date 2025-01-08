/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomlimon <tom.limon@>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/22 17:12:51 by tomlimon          #+#    #+#             */
/*   Updated: 2025/01/07 15:14:35 by tomlimon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/philo.h"

int ft_atoi(char *str)
{
	int i;
	int result;
	int sign;

	sign = 1;
	result = 0;
	i = 0;

	if (str[i] == '-')
	{
		sign = -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		result = result * 10 + str[i] - '0';
		i++;
	}
	return (result * sign);
}
void ft_cleanup(t_table *table)
{
    int i;

    i = 0;
    while (i < table->nb_philos)
    {
        pthread_join(table->philos[i].thread, NULL); // Attendre la fin des threads
        i++;
    }
    i = 0;
    while (i < table->nb_philos)
    {
        pthread_mutex_destroy(&table->forks[i]); // Détruire les mutex
        i++;
    }
    free(table->philos);
    free(table->forks);
}

long long ft_get_time(void)
{
    struct timeval tv;
    long long milliseconds;

    gettimeofday(&tv, NULL);
    milliseconds = (tv.tv_sec * 1000LL) + (tv.tv_usec / 1000);
    return milliseconds;
}
void print_status(t_table *table, int philo_id, char *message)
{
    pthread_mutex_lock(&table->write_mutex);
    if (table->simulation_running || ft_strcmp(message, "died") == 0)
    {
        printf("%lld %d %s\n", get_timestamp(table), philo_id, message);
    }
    pthread_mutex_unlock(&table->write_mutex);
}

long long get_timestamp(t_table *table)
{
    struct timeval current_time;
    gettimeofday(&current_time, NULL);
    return ((current_time.tv_sec * 1000 + current_time.tv_usec / 1000) - table->start_time);
}
int ft_strcmp(const char *s1, const char *s2)
{
    size_t i;

    i = 0;
    while (s1[i] && s2[i] && s1[i] == s2[i])
        i++;
    return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}
