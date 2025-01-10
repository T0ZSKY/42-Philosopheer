/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomlimon <tom.limon@>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/22 17:07:18 by tomlimon          #+#    #+#             */
/*   Updated: 2025/01/09 14:35:53 by tomlimon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>

typedef struct s_table t_table;

typedef struct s_philosopher
{
    int             id;
    int             meals_eaten;
    long long       last_meal_time;
    pthread_mutex_t *left_fork;
    pthread_mutex_t *right_fork;
    pthread_mutex_t mutex;
    t_table         *table;
    pthread_t       thread;
}   t_philosopher;

typedef struct s_table
{
    int                 nb_philos;
    int                 time_to_die;
    int                 time_to_eat;
    int                 time_to_sleep;
    int                 max_meals;
    int                 meals_taken;
    int                 simulation_running;
    pthread_mutex_t     status_mutex;
    pthread_mutex_t     write_mutex;
    pthread_mutex_t     meals_mutex;
    long long           start_time;
    t_philosopher       *philos;
    pthread_mutex_t     *forks;
}   t_table;


///declaration function

int ft_atoi(char *str);
void ft_cleanup(t_table *table);
long long ft_get_time(void);
void print_status(t_table *table, int philo_id, char *message);
long long get_timestamp(t_table *table);
int ft_strcmp(const char *s1, const char *s2);
void ft_smart_sleep(long long time, t_table *table);
int	ft_init_philo(t_table *table, int nb, char **argv, int argc);

#endif
