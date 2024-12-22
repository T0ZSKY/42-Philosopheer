/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomlimon <tom.limon@>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/22 17:07:18 by tomlimon          #+#    #+#             */
/*   Updated: 2024/12/22 17:14:42 by tomlimon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>

typedef struct s_philosopher
{
    int             id;
    int             meals_eaten;
    long long       last_meal_time;
    pthread_mutex_t *left_fork;
    pthread_mutex_t *right_fork;
}   t_philosopher;

typedef struct s_table
{
    int                 nb_philos;
    int                 time_to_die;
    int                 time_to_eat;
    int                 time_to_sleep;
    t_philosopher       *philos;
    pthread_mutex_t     *forks;
}   t_table;


///declaration function

int ft_atoi(char *str);

#endif
