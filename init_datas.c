#include "philo.h"

void	init_data(t_data *data, int argc, char **argv)
{
	pthread_mutex_init(&data->data_mutex, NULL);
	pthread_mutex_init(&data->print_mutex, NULL);
	data->philo_ready = 0;
	data->philo_nbr = ft_atol(argv[1]);
	data->time_to_die = ft_atol(argv[2]) * 1e3;
	data->time_to_eat = ft_atol(argv[3]) * 1e3;
	data->time_to_sleep = ft_atol(argv[4]) * 1e3;
	data->end = false;
	data->sync = false;
	if (argc == 6)
		data->meals_limit = ft_atol(argv[5]);
	else
		data->meals_limit = -1;
	philo_init(data);
}

void simulation_start(t_data *data)
{
	int i;

	i = 0;
	while (i < data->philo_nbr)
	{
		pthread_create(&data->philos[i].thread_id, NULL, &philo_routine, &data->philos[i]);
		i++;
	}
	set_long(&data->data_mutex, &data->start_time, get_current_time(MILLISECOND));
	pthread_create(&data->monitor_thread, NULL, &monitor_routine, data);
	set_bool(&data->data_mutex, &data->sync, true);
	i = 0;
	while (i < data->philo_nbr)
	{
		pthread_join(data->philos[i].thread_id, NULL);
		i++;
	}
	set_bool(&data->data_mutex, &data->end, true);
	pthread_join(data->monitor_thread, NULL);
}

void philo_init(t_data *data)
{
	int i;
	
	i = 0;
	if (data->philo_nbr == 1)
	{
		one_philo(data);
		return ;
	}
	data->forks = malloc(sizeof(t_fork) * data->philo_nbr);
	if (!data->forks)
		exit (1);
	while (i < data->philo_nbr)
	{
		data->forks[i].fork_id = i;
		if (pthread_mutex_init(&data->forks[i].fork, NULL))
			exit (1);
		i++;
	}
	data->philos = malloc(sizeof(t_philo) * data->philo_nbr);
	if (!data->philos)
		exit (1);
	i = 0;
	while (i < data->philo_nbr)
	{
		data->philos[i].data = data;
		data->philos[i].id = i + 1;
		data->philos[i].meals_counter = 0;
		data->philos[i].full = false;
		pthread_mutex_init(&data->philos[i].mutex, NULL);
		take_forks(&data->philos[i], data->forks, i);
		i++;
	}
}

void take_forks(t_philo *philo, t_fork *forks, int position)
{
	int nb_philo;

	nb_philo = philo->data->philo_nbr;
	philo->right_fork = &forks[(position + 1) % nb_philo];
	philo->left_fork = &forks[position];
	if (philo->id % 2)
	{
		philo->left_fork = &forks[position];
		philo->right_fork = &forks[(position + 1) % nb_philo];
	}
}

