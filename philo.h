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
	long	fork_id;	
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
	pthread_mutex_t mutex;
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
	bool sync;
	pthread_mutex_t data_mutex;
	pthread_t monitor_thread;
	pthread_mutex_t print_mutex;
	t_fork *forks;
	t_philo *philos;
}	t_data;

typedef enum e_time_code
{
	SECOND,
	MILLISECOND,
	MICROSECOND,
}	t_time_code;

typedef enum e_philo_status
{
	EATING,
	SLEEPING,
	THINKING,
	TAKE_FIRST_FORK,
	TAKE_SECOND_FORK,
	DEAD,
}	t_philo_status;

void *philo_routine(void *arg);
int	init_data(t_data *data, int argc, char **argv);
long ft_atol(const char *str);
long	get_current_time(t_time_code code);
int philo(t_data *data);
void    set_bool(pthread_mutex_t *mutex, bool *var, bool value);
bool    get_bool(pthread_mutex_t *mutex, bool *var);
void    set_long(pthread_mutex_t *mutex, long *var, long value);
long    get_long(pthread_mutex_t *mutex, long *var);
bool   is_philo_dead(t_philo *philo);
bool simulation_finished(t_data *data);
void wait_all_threads(t_data *data);
void precise_sleep(long time ,t_data *data);
void write_status(t_philo_status status, t_philo *philo);
void eating(t_philo *philo);
void thinking(t_philo *philo);
void sleeping(t_philo *philo);
void *monitor_routine(void *arg);

#endif