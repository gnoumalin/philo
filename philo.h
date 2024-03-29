#ifndef PHILO_H
# define PHILO_H
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdbool.h>
# include <sys/time.h>

typedef struct s_data t_data;

typedef struct s_fork
{
	pthread_mutex_t	fork;
	int	fork_id;	
}	t_fork;

typedef struct s_philo
{
	int id;
	long meals_counter;
	bool full;
	long last_meal;
	t_fork *left_fork;
	t_fork *right_fork;
	pthread_t thread_id;
	t_data *data;
}	t_philo;

typedef struct s_data
{
	long	philo_nbr;
	long	time_to_die;
	long	time_to_eat;
	long	time_to_sleep;
	long meals_limit;
	long start_time;
	bool end;
	t_fork *forks;
	t_philo *philos;
}	t_data;

void *philo_routine(void *arg);
int	init_data(t_data *data, int argc, char **argv);
long ft_atol(const char *str);
size_t	get_current_time(void);
int philo(t_data *data);

#endif