#include "philo.h"

void eating(t_philo *philo)
{
    pthread_mutex_lock(&philo->data->forks[philo->right_fork->fork_id].fork);
    write_status(TAKE_FIRST_FORK, philo);
    pthread_mutex_lock(&philo->data->forks[philo->left_fork->fork_id].fork);
    write_status(TAKE_SECOND_FORK, philo);
    set_long(&philo->mutex, &philo->last_meal, get_current_time(MILLISECOND)); 
    philo->meals_counter++;
    write_status(EATING, philo);
    precise_sleep(philo->data->time_to_eat, philo->data);
    if (philo->data->meals_limit > 0 && philo->meals_counter == philo->data->meals_limit)
        set_bool(&philo->mutex, &philo->full, true);
    pthread_mutex_unlock(&philo->data->forks[philo->right_fork->fork_id].fork);
    pthread_mutex_unlock(&philo->data->forks[philo->left_fork->fork_id].fork);
}

void thinking(t_philo *philo, bool desync)
{
    long t_think;
	long t_sleep;
	long t_eat;
    
    if (!desync)
        write_status(THINKING, philo);
    if (philo->data->philo_nbr % 2)
        return ;
	t_eat = philo->data->time_to_eat;
	t_sleep = philo->data->time_to_sleep;
	t_think = ((t_eat * 2) - t_sleep) / 1e3;
    if (t_think < 0)
        t_think = 0;
    precise_sleep(t_think * 0.42, philo->data);
}

void sleeping(t_philo *philo)
{
    write_status(SLEEPING, philo);
    precise_sleep(philo->data->time_to_sleep, philo->data);
}