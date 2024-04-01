#include "philo.h"

void	init_data(t_data *data, int argc, char **argv)
{
	pthread_mutex_init(&data->data_mutex, NULL);
	pthread_mutex_init(&data->print_mutex, NULL);
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
}

void one_philo(t_data *data)
{
	write_status(TAKE_FIRST_FORK, &data->philos[0]);
	precise_sleep(data->time_to_die, data);
	write_status(DEAD, &data->philos[0]);
}

void *philo_routine(void *arg)
{
	t_philo *philo = (t_philo *)arg;

	wait_all_threads(philo->data);
	//desync(philo);
	set_long(&philo->mutex, &philo->data->start_time, get_current_time(MILLISECOND));
	set_long(&philo->mutex, &philo->last_meal, get_current_time(MILLISECOND));
	while (!simulation_finished(philo->data))
	{
		if (philo->full)
			break ;
		eating(philo);
		sleeping(philo);
		write_status(SLEEPING, philo);
		precise_sleep(philo->data->time_to_sleep, philo->data);
		thinking(philo, false);
	}
	return NULL;
}

void take_forks(t_philo *philo, t_fork *forks, int position)
{
	int nb_philo;

	nb_philo = philo->data->philo_nbr;
	philo->left_fork = &forks[(position + 1) % nb_philo];
	philo->right_fork = &forks[position];
	if (philo->id % 2)
	{
		philo->left_fork = &forks[position];
		philo->right_fork = &forks[(position + 1) % nb_philo];
	}
}

void desync(t_philo *philo)
{
	if (philo->data->philo_nbr % 2)
	{
		if (philo->id % 2)
			precise_sleep(3e4, philo->data);
	}
	else
	{
		if (philo->id % 2)
			thinking(philo, true);
	}
}

int philo(t_data *data)
{
	int i;
	
	i = 0;
	if (data->philo_nbr == 1)
	{
		one_philo(data);
		return (0);
	}
	data->forks = malloc(sizeof(t_fork) * data->philo_nbr);
	if (!data->forks)
		return (1);
	while (i < data->philo_nbr)
	{
		data->forks[i].fork_id = i;
		if (pthread_mutex_init(&data->forks[i].fork, NULL))
			return (1);
		i++;
	}
	data->philos = malloc(sizeof(t_philo) * data->philo_nbr);
	if (!data->philos)
		return (1);
	i = 0;
	while (i < data->philo_nbr)
	{
		data->philos[i].data = data;
		set_long(&data->philos[i].mutex, &data->philos[i].last_meal, get_current_time(MILLISECOND));
		data->philos[i].id = i + 1;
		data->sync = false;
		data->philos[i].meals_counter = 0;
		data->philos[i].full = false;
		take_forks(&data->philos[i], data->forks, i);
		pthread_mutex_init(&data->philos[i].mutex, NULL);
		data->philos[i].data = data;
		if (pthread_create(&data->philos[i].thread_id, NULL, &philo_routine, &data->philos[i]) == -1)
			return (1);	
		i++;
	}
	pthread_create(&data->monitor_thread, NULL, &monitor_routine, data);
	data->start_time = get_current_time(MILLISECOND);
	set_bool(&data->data_mutex, &data->sync, true);
	i = 0;
	while (i < data->philo_nbr)
	{
		pthread_join(data->philos[i].thread_id, NULL);
		i++;
	}
	pthread_join(data->monitor_thread, NULL);
	return (0);
}

int main(int argc, char **argv)
{
	t_data data;
	if (init_parsing(argc, argv) == 1)
		return (1);
	init_data(&data, argc, argv);
	if (philo(&data))
		return (1);
	clear_data(&data);
}