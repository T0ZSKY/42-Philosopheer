/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomlimon <tom.limon@>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/22 17:12:51 by tomlimon          #+#    #+#             */
/*   Updated: 2025/01/06 11:50:20 by tomlimon         ###   ########.fr       */
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
    free(table->philos);  // Libérer la mémoire allouée pour les philosophes
    free(table->forks);   // Libérer la mémoire allouée pour les fourchettes
}
