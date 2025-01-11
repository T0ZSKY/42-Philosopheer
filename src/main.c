/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomlimon <tom.limon@>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 08:12:53 by tomlimon          #+#    #+#             */
/*   Updated: 2025/01/11 13:48:36 by tomlimon         ###   ########.fr       */
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

static int	validate_and_init(t_table *table, int argc, char **argv)
{
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
	if (ft_init_philo(table, atoi(argv[1]), argv, argc) == -1)
	{
		write(1, "initialization error\n", 22);
		return (-1);
	}
	return (0);
}

static int	create_threads(t_table *table, pthread_t *supervisor_thread)
{
	ft_init_thread(table);
	if (pthread_create(supervisor_thread, NULL, ft_supervisor, table) != 0)
	{
		printf("Erreur création thread superviseur\n");
		ft_cleanup(table);
		return (-1);
	}
	return (0);
}

int	main(int argc, char **argv)
{
	t_table		table;
	pthread_t	supervisor_thread;
	int			i;

	if (ft_verif(argv, argc) == 1)
	{
		printf("Erreur argument\n");
		return (-1);
	}
	if (validate_and_init(&table, argc, argv) == -1)
		return (-1);
	if (create_threads(&table, &supervisor_thread) == -1)
		return (-1);
	pthread_join(supervisor_thread, NULL);
	i = 0;
	while (i < table.nb_philos)
	{
		if (pthread_join(table.philos[i].thread, NULL) != 0)
			printf("Erreur lors du join du thread %d\n", i + 1);
		i++;
	}
	ft_cleanup(&table);
	return (0);
}
