/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomlimon <tom.limon@>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 08:12:53 by tomlimon          #+#    #+#             */
/*   Updated: 2025/01/08 16:47:11 by tomlimon         ###   ########.fr       */
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

int ft_init_philo(t_table *table, int nb, char **argv, int argc)
{
    int i;

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
    struct timeval tv;
    gettimeofday(&tv, NULL);
    table->start_time = tv.tv_sec * 1000 + tv.tv_usec / 1000;

    if (pthread_mutex_init(&table->write_mutex, NULL) != 0)
    {
        perror("error mutex");
        return (-1);
    }
    i = 0;
    while (i < nb)
    {
        table->philos[i].id = i + 1;
        table->philos[i].meals_eaten = 0;
        table->philos[i].last_meal_time = ft_get_time();
        table->philos[i].left_fork = &table->forks[i];
        table->philos[i].right_fork = &table->forks[(i + 1) % nb];
        table->philos[i].table = table;
        table->simulation_running = 1;
        i++;
    }
    return (0);
}

void *ft_supervisor(void *arg)
{
    t_table *table = (t_table *)arg;

    while (1)
    {
        int meals_completed = 1;

        for (int i = 0; i < table->nb_philos; i++)
        {
            long long current_time = get_timestamp(table);
            long long time_since_last_meal = current_time -
                                            (table->philos[i].last_meal_time - table->start_time);

            if (time_since_last_meal > table->time_to_die)
            {
                pthread_mutex_lock(&table->status_mutex);
                table->simulation_running = 0;
                pthread_mutex_unlock(&table->status_mutex);

                print_status(table, table->philos[i].id, "died");
                return NULL;
            }

            if (table->max_meals != -1 && table->philos[i].meals_eaten < table->max_meals)
                meals_completed = 0;
        }

        if (table->max_meals != -1 && meals_completed)
        {
            pthread_mutex_lock(&table->status_mutex);
            table->simulation_running = 0;
            pthread_mutex_unlock(&table->status_mutex);
            return NULL;
        }

        usleep(1000);
    }

    return NULL;
}

void *ft_philo_routine(void *arg)
{
    t_philosopher *philo = (t_philosopher *)arg;
    t_table *table = philo->table;

    if (philo->id % 2 == 0)
        usleep(1000);

    if (table->nb_philos == 1)
    {
        print_status(table, philo->id, "has taken a fork");
        usleep(table->time_to_die * 1000);
        return NULL;
    }

    while (1)
    {
        pthread_mutex_lock(&table->status_mutex);
        if (!table->simulation_running)
        {
            pthread_mutex_unlock(&table->status_mutex);
            break;
        }
        pthread_mutex_unlock(&table->status_mutex);

        // Prendre les fourchettes
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

        // Manger
        pthread_mutex_lock(&table->meals_mutex);
        philo->last_meal_time = get_timestamp(table) + table->start_time;
        print_status(table, philo->id, "is eating");
        if (table->max_meals != -1)
            philo->meals_eaten++;
        pthread_mutex_unlock(&table->meals_mutex);

        ft_smart_sleep(table->time_to_eat, table);

        // Relâcher les fourchettes
        pthread_mutex_unlock(philo->left_fork);
        pthread_mutex_unlock(philo->right_fork);

        print_status(table, philo->id, "is sleeping");
        ft_smart_sleep(table->time_to_sleep, table);

        print_status(table, philo->id, "is thinking");

        pthread_mutex_lock(&table->status_mutex);
        if (!table->simulation_running)
        {
            pthread_mutex_unlock(&table->status_mutex);
            break;
        }
        pthread_mutex_unlock(&table->status_mutex);
    }
    return NULL;
}

void ft_init_thread(t_table *table)
{
    int i;

    i = 0;
    while (i < table->nb_philos)
    {
        if (pthread_create(&table->philos[i].thread, NULL, ft_philo_routine, &table->philos[i]) != 0)
        {
            printf("Erreur création thread pour le philosophe %d\n", i + 1);
            while (i > 0)
            {
                pthread_cancel(table->philos[--i].thread);
            }
            return;
        }
        i++;
    }
}

int main(int argc, char **argv)
{
    t_table table;
    pthread_t supervisor_thread;

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
        return (-1);
    }

    pthread_join(supervisor_thread, NULL);

    for (int i = 0; i < table.nb_philos; i++)
    {
        pthread_join(table.philos[i].thread, NULL);
    }

    // Libérer les ressources
    ft_cleanup(&table);

    return (0);
}
