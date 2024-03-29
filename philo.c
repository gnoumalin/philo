#include "philo.h"

size_t	get_current_time(void)
{
	struct timeval	time;

	if (gettimeofday(&time, NULL) == -1)
		write(2, "gettimeofday() error\n", 22);
	return (time.tv_sec * 1000 + time.tv_usec / 1000);
}

long ft_atol(const char *str)
{
	long res;
	int sign;
	int i;

	res = 0;
	sign = 1;
	i = 0;
	while (str[i] == ' ' || str[i] == '\t' || str[i] == '\n' || str[i] == '\v' || str[i] == '\f' || str[i] == '\r')
		i++;
	if (str[i] == '-')
	{
		sign = -1;
		i++;
	}
	else if (str[i] == '+')
		i++;
	while (str[i] >= '0' && str[i] <= '9')
	{
		res = res * 10 + (str[i] - '0');
		i++;
	}
	return (res * sign);
}

int	init_data(t_data *data, int argc, char **argv)
{
	data->philo_nbr = ft_atol(argv[1]);
	data->time_to_die = ft_atol(argv[2]);
	data->time_to_eat = ft_atol(argv[3]);
	data->time_to_sleep = ft_atol(argv[4]);
	if (argc == 6)
	{	
		data->meals_limit = ft_atol(argv[5]);
		if (data->meals_limit <= 0)
			return (1);
	}
	else
		data->meals_limit = -1;
	if (data->philo_nbr < 2 || data->philo_nbr > __INT_MAX__ || data->meals_limit > __INT_MAX__)
		return (1);
	return (0);
}

void *philo_routine(void *arg)
{
	t_philo *philo = (t_philo *)arg;
	while (1)
	{
		pthread_mutex_lock(&philo->left_fork->fork);
		printf("%ld %d has taken a fork\n",get_current_time(), philo->id);
		pthread_mutex_lock(&philo->right_fork->fork);
		printf("%ld %d has taken a fork\n",get_current_time(), philo->id);
		philo->last_meal = get_current_time();
		printf("%ld %d is eating\n", get_current_time(), philo->id);
		usleep(philo->data->time_to_eat);
		pthread_mutex_unlock(&philo->left_fork->fork);
		pthread_mutex_unlock(&philo->right_fork->fork);
		printf("%ld %d is sleeping\n",get_current_time(), philo->id);
		usleep(philo->data->time_to_sleep);
		printf("%ld %d is thinking\n", get_current_time(), philo->id);
	}
	return NULL;
}

int philo(t_data *data)
{
	int i;
	
	i = 0;
	data->start_time = get_current_time();
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
		data->philos[i].id = i;
		data->philos[i].meals_counter = 0;
		data->philos[i].full = false;
		data->philos[i].last_meal = get_current_time();
		data->philos[i].left_fork = &data->forks[i];
		if (i == data->philo_nbr - 1)
			data->philos[i].right_fork = &data->forks[0];
		else
			data->philos[i].right_fork = &data->forks[i + 1];
		if (data->philos->id % 2)
		{
			if (i == data->philo_nbr - 1)
				data->philos[i].right_fork = &data->forks[0];
			else
				data->philos[i].right_fork = &data->forks[i + 1];
			data->philos[i].left_fork = &data->forks[i];
		}
		data->philos[i].data = data;
		if (pthread_create(&data->philos[i].thread_id, NULL, &philo_routine, &data->philos[i]))
			return (1);
		usleep(1000);	
		i++;
	}
	return (0);
}

int main(int argc, char **argv)
{
	t_data data;
	
	if (argc != 5 && argc != 6)
	{
		printf("Error: wrong number of arguments\n");
		return (1);
	}
	if (init_data(&data, argc, argv))
		return (1);
	if (philo(&data))
		return (1);
}