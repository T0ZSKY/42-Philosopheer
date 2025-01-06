/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomlimon <tom.limon@>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 08:12:53 by tomlimon          #+#    #+#             */
/*   Updated: 2025/01/06 16:54:23 by tomlimon         ###   ########.fr       */
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
    table->time_to_sleep = ft_atoi(argv[4]);

    table->forks = malloc(sizeof(pthread_mutex_t) * nb);
    if (!table->forks)
    {
        perror("Erreur d'allocation pour les fourchettes");
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

    i = 0;
    while (i < nb)
    {
        table->philos[i].id = i + 1;
        table->philos[i].meals_eaten = 0;
        table->philos[i].last_meal_time = 0;
        table->philos[i].left_fork = &table->forks[i];
        table->philos[i].right_fork = &table->forks[(i + 1) % nb];
        table->philos[i].table = table;
        i++;
    }
    return (0);
}

long long ft_get_time(void)
{
    struct timeval tv;
    long long milliseconds;

    gettimeofday(&tv, NULL);
    milliseconds = (tv.tv_sec * 1000LL) + (tv.tv_usec / 1000);
    return milliseconds;
}

void *ft_philo_routine(void *arg)
{
    t_philosopher *philo = (t_philosopher *)arg;

    if (philo->id % 2 == 0)
        usleep(100);

    while (1)
    {
        // Prise des fourchettes (avec ordre alterné)
        if (philo->id % 2 == 0)
        {
            pthread_mutex_lock(philo->right_fork);
            printf("%d has taken a fork\n", philo->id);
            pthread_mutex_lock(philo->left_fork);
            printf("%d has taken a fork\n", philo->id);
        }
        else
        {
            pthread_mutex_lock(philo->left_fork);
            printf("%d has taken a fork\n", philo->id);
            pthread_mutex_lock(philo->right_fork);
            printf("%d has taken a fork\n", philo->id);
        }

        if (philo->last_meal_time - philo->table->time_to_eat >= philo->table->time_to_die)
        {
            printf("%d died\n", philo->id);
            pthread_mutex_unlock(philo->right_fork);
            pthread_mutex_unlock(philo->left_fork);
            break ;
        }
        // Manger
        printf("%d is eating\n", philo->id);
        philo->last_meal_time = ft_get_time();
        philo->meals_eaten++;
        usleep(philo->table->time_to_eat * 1000);

        // Relâcher les fourchettes
        pthread_mutex_unlock(philo->right_fork);
        pthread_mutex_unlock(philo->left_fork);

        // Dormir
        printf("%d is sleeping\n", philo->id);
        usleep(philo->table->time_to_sleep * 1000);

        printf("%d is thinking\n", philo->id);

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
            // Libérer les ressources allouées
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

    if (argc < 5)
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
    ft_cleanup(&table);

    return (0);
}
