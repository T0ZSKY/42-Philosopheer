/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomlimon <tom.limon@>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 14:32:27 by tomlimon          #+#    #+#             */
/*   Updated: 2025/01/09 14:53:21 by tomlimon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/philo.h"

int	ft_init_philo(t_table *table, int nb, char **argv, int argc)
{
	int				i;
	struct timeval	tv;

	i = 0;
	table->nb_philos = nb;
	table->time_to_die = ft_atoi(argv[2]);
	table->time_to_eat = ft_atoi(argv[3]);
	table->time_to_sleep = ft_atoi(argv[4]);
	table->forks = malloc(sizeof(pthread_mutex_t) * nb);
	if (!table->forks)
	{
		perror("Erreur d'allocation pour les fourchettes");
		return (-1);
	}
	if (argc == 6)
	{
		table->max_meals = ft_atoi(argv[5]);
		if (table->max_meals <= 0)
		{
			write(1, "invalid max meals\n", 19);
			return (-1);
		}
	}
	else
		table->max_meals = -1;
	table->meals_taken = 0;
	if (pthread_mutex_init(&table->meals_mutex, NULL) != 0)
	{
		perror("Erreur mutex meals");
		return (-1);
	}
	while (i < nb)
	{
		if (pthread_mutex_init(&table->forks[i], NULL) != 0)
		{
			perror("Erreur d'initialisation de mutex pour une fourchette");
			return (-1);
		}
		i++;
	}
	table->philos = malloc(sizeof(t_philosopher) * nb);
	if (!table->philos)
	{
		perror("Erreur d'allocation pour les philosophes");
		free(table->forks);
		return (-1);
	}
	if (pthread_mutex_init(&table->status_mutex, NULL) != 0)
	{
		perror("Erreur mutex status");
		return (-1);
	}
	gettimeofday(&tv, NULL);
	table->start_time = tv.tv_sec * 1000 + tv.tv_usec / 1000;
	if (pthread_mutex_init(&table->write_mutex, NULL) != 0)
	{
		perror("error mutex");
		return (-1);
	}
	i = 0;
	table->simulation_running = 1;
	while (i < nb)
	{
		table->philos[i].id = i + 1;
		table->philos[i].meals_eaten = 0;
		table->philos[i].last_meal_time = ft_get_time();
		table->philos[i].left_fork = &table->forks[i];
		table->philos[i].right_fork = &table->forks[(i + 1) % nb];
		table->philos[i].table = table;
		pthread_mutex_init(&table->philos[i].mutex, NULL);
		i++;
	}
	return (0);
}
