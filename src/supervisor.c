/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   supervisor.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomlimon <tom.limon@>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 13:38:43 by tomlimon          #+#    #+#             */
/*   Updated: 2025/01/11 13:31:01 by tomlimon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/philo.h"

int	check_philosopher_death(t_table *table, int i, long long current_time)
{
	long long	time_since_last_meal;
	long long	last_meal_time;

	pthread_mutex_lock(&table->philos[i].mutex);
	last_meal_time = table->philos[i].last_meal_time;
	pthread_mutex_unlock(&table->philos[i].mutex);
	time_since_last_meal = current_time - (last_meal_time - table->start_time);
	if (time_since_last_meal > table->time_to_die)
	{
		pthread_mutex_lock(&table->status_mutex);
		table->simulation_running = 0;
		pthread_mutex_unlock(&table->status_mutex);
		print_status(table, table->philos[i].id, "died");
		return (1);
	}
	return (0);
}

static int	check_all_meals_completed(t_table *table, int nb_philos)
{
	int	i;
	int	meals_completed;

	meals_completed = 1;
	i = 0;
	while (i < nb_philos)
	{
		pthread_mutex_lock(&table->philos[i].mutex);
		if (table->max_meals != -1
			&& table->philos[i].meals_eaten < table->max_meals)
			meals_completed = 0;
		pthread_mutex_unlock(&table->philos[i].mutex);
		i++;
	}
	if (table->max_meals != -1 && meals_completed)
	{
		pthread_mutex_lock(&table->status_mutex);
		table->simulation_running = 0;
		pthread_mutex_unlock(&table->status_mutex);
		return (1);
	}
	return (0);
}

void	*ft_supervisor(void *arg)
{
	t_table		*table;
	int			i;
	long long	current_time;

	table = (t_table *)arg;
	while (1)
	{
		i = 0;
		while (i < table->nb_philos)
		{
			current_time = get_timestamp(table);
			if (check_philosopher_death(table, i, current_time))
				return (NULL);
			i++;
		}
		if (check_all_meals_completed(table, table->nb_philos))
			return (NULL);
		usleep(1000);
	}
	return (NULL);
}
