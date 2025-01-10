/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomlimon <tom.limon@>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 08:12:53 by tomlimon          #+#    #+#             */
/*   Updated: 2025/01/10 13:39:04 by tomlimon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/philo.h"

int	ft_validate_args(char **argv)
{
	int	i;

	i = 1;
	while (argv[i])
	{
		if (atoi(argv[i]) <= 0)
			return (-1);
		i++;
	}
	return (0);
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
		print_status(table, philo->id, "has taken a fork");
		usleep(table->time_to_die * 1000);
		return (NULL);
	}
	while (1)
	{
		pthread_mutex_lock(&table->status_mutex);
		if (!table->simulation_running)
		{
			pthread_mutex_unlock(&table->status_mutex);
			break ;
		}
		pthread_mutex_unlock(&table->status_mutex);
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
		pthread_mutex_lock(&table->status_mutex);
		if (!table->simulation_running)
		{
			pthread_mutex_unlock(&table->status_mutex);
			break ;
		}
		pthread_mutex_unlock(&table->status_mutex);
	}
	return (NULL);
}

void	ft_init_thread(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->nb_philos)
	{
		if (pthread_create(&table->philos[i].thread,
				NULL, ft_philo_routine, &table->philos[i]) != 0)
		{
			printf("Erreur création thread pour le philosophe %d\n", i + 1);
			while (i > 0)
			{
				pthread_cancel(table->philos[--i].thread);
			}
			return ;
		}
		i++;
	}
}

int	main(int argc, char **argv)
{
	t_table		table;
	pthread_t	supervisor_thread;
	int			i;

	if (argc < 5 || argc > 6)
	{
		write(1, "error argument\n", 16);
		return (-1);
	}
	if (ft_validate_args(argv) == -1)
	{
		write(1, "invalid argument\n", 17);
		return (-1);
	}
	if (ft_init_philo(&table, atoi(argv[1]), argv, argc) == -1)
	{
		write(1, "initialization error\n", 22);
		return (-1);
	}
	ft_init_thread(&table);
	if (pthread_create(&supervisor_thread, NULL, ft_supervisor, &table) != 0)
	{
		printf("Erreur création thread superviseur\n");
		ft_cleanup(&table);
		return (-1);
	}
	pthread_join(supervisor_thread, NULL);
	i = 0;
	while (i < table.nb_philos)
	{
		if (pthread_join(table.philos[i].thread, NULL) != 0)
		{
			printf("Erreur lors du join du thread %d\n", i + 1);
		}
		i++;
	}
	ft_cleanup(&table);
	return (0);
}
