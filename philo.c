#include "philo.h"

void one_philo(t_data *data)
{
	write_status(TAKE_FIRST_FORK, &data->philos[0]);
	precise_sleep(data->time_to_die, data);
	write_status(DEAD, &data->philos[0]);
}

void	increment_value(pthread_mutex_t *mutex, long *philo_ready)
{	
	pthread_mutex_lock(mutex);
	(*philo_ready)++;
	pthread_mutex_unlock(mutex);
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

void *philo_routine(void *arg)
{
	t_philo *philo = (t_philo *)arg;

	wait_all_threads(philo->data);
	set_long(&philo->mutex, &philo->last_meal, get_current_time(MILLISECOND));
	increment_value(&philo->data->data_mutex, &philo->data->philo_ready);
	desync(philo);
	while (!simulation_finished(philo->data))
	{
		if (get_bool(&philo->mutex, &philo->full))
			break ;
		eating(philo);
		sleeping(philo);
		thinking(philo, false);
	}
	return NULL;
}


void philo_start(t_data *data)
{
	int i;

	i = 0;
	while (i < data->philo_nbr)
	{
		pthread_create(&data->philos[i].thread_id, NULL, &philo_routine, &data->philos[i]);
		i++;
	}
}

int main(int argc, char **argv)
{
	t_data data;
	if (init_parsing(argc, argv) == 1)
		return (1);
	init_data(&data, argc, argv);
	simulation_start(&data);
	clear_data(&data);
}