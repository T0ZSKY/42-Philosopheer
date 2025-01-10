/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomlimon <tom.limon@>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 16:55:54 by tomlimon          #+#    #+#             */
/*   Updated: 2025/01/10 17:12:36 by tomlimon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/philo.h"

static int	check_simulation_status(t_table *table)
{
	pthread_mutex_lock(&table->status_mutex);
	if (!table->simulation_running)
	{
		pthread_mutex_unlock(&table->status_mutex);
		return (0);
	}
	pthread_mutex_unlock(&table->status_mutex);
	return (1);
}

static void	handle_single_philo(t_table *table, t_philosopher *philo)
{
	print_status(table, philo->id, "has taken a fork");
	usleep(table->time_to_die * 1000);
}

static void	take_forks(t_philosopher *philo, t_table *table)
{
	if (philo->id % 2 == 0)
	{
		pthread_mutex_lock(philo->right_fork);
		print_status(table, philo->id, "has taken a fork");
		pthread_mutex_lock(philo->left_fork);
		print_status(table, philo->id, "has taken a fork");
	}
	else
	{
		pthread_mutex_lock(philo->left_fork);
		print_status(table, philo->id, "has taken a fork");
		pthread_mutex_lock(philo->right_fork);
		print_status(table, philo->id, "has taken a fork");
	}
}

static void	philo_actions(t_philosopher *philo, t_table *table)
{
	pthread_mutex_lock(&philo->mutex);
	philo->last_meal_time = get_timestamp(table) + table->start_time;
	print_status(table, philo->id, "is eating");
	if (table->max_meals != -1)
		philo->meals_eaten++;
	pthread_mutex_unlock(&philo->mutex);
	ft_smart_sleep(table->time_to_eat, table);
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
	print_status(table, philo->id, "is sleeping");
	ft_smart_sleep(table->time_to_sleep, table);
	print_status(table, philo->id, "is thinking");
}

void	*ft_philo_routine(void *arg)
{
	t_philosopher	*philo;
	t_table			*table;

	philo = (t_philosopher *)arg;
	table = philo->table;
	if (philo->id % 2 == 0)
		usleep(1000);
	if (table->nb_philos == 1)
	{
		handle_single_philo(table, philo);
		return (NULL);
	}
	while (1)
	{
		if (!check_simulation_status(table))
			break ;
		take_forks(philo, table);
		philo_actions(philo, table);
		if (!check_simulation_status(table))
			break ;
	}
	return (NULL);
}
