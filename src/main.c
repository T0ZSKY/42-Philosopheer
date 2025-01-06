/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomlimon <tomlimon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 08:12:53 by tomlimon          #+#    #+#             */
/*   Updated: 2025/01/06 10:45:54 by tomlimon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/philo.h"

int ft_validate_args(char **argv)
{
    int i;

    i = 1;
    while (argv[i])
    {
        if (atoi(argv[i]) <= 0)
            return (-1);
        i++;
    }
    return (0);
}

int ft_init_philo(t_table *table, int nb, char **argv)
{
    int i;

	i = 0;
    table->nb_philos = nb;
    table->time_to_die = ft_atoi(argv[2]);
    table->time_to_eat = ft_atoi(argv[3]);
	if (argv[4])
		table->time_to_sleep = ft_atoi(argv[4]);

    table->forks = malloc(sizeof(pthread_mutex_t) * nb);
    if (!table->forks)
        return (-1);
    while (i < nb)
	{
        pthread_mutex_init(&table->forks[i], NULL);
		i++;
	}

    // Allouer les philosophes
    table->philos = malloc(sizeof(t_philosopher) * nb);
    if (!table->philos)
        return (-1);
	i = 0;
    while (i < nb)
    {
        table->philos[i].id = i + 1;
        table->philos[i].meals_eaten = 0;
        table->philos[i].last_meal_time = 0;
        table->philos[i].left_fork = &table->forks[i];
        table->philos[i].right_fork = &table->forks[(i + 1) % nb];
		i++;
    }
    return (0);
}

void ft_philo_routine()
{
    
}

void ft_init_thread(t_table *table)
{
    int i;

    i = 0;
    while (i < table->nb_philos)
    {
        if (pthread_create(&table->philo[i].thread, NULL, ft_philo_routine, &table->philo[i] != 0))
        {
            printf("Erreur thread creation");
            return ;
        }
        pthread_detach(table->philo[i].thread);
        i++;
    }
}

int main(int argc, char **argv)
{
    t_table table;

    if (argc < 4)
    {
        write(1, "error argument\n", 16);
        return (-1);
    }
    if (ft_validate_args(argv) == -1)
    {
        write(1, "invalid argument\n", 17);
        return (-1);
    }
    if (ft_init_philo(&table, atoi(argv[1]), argv) == -1)
    {
        write(1, "initialization error\n", 22);
        return (-1);
    }
    ft_init_thread(&table);
    
    return (0);
}
