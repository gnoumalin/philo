#include "philo.h"

void write_status(t_philo_status status, t_philo *philo)
{
    long   current_time;
    
    current_time = get_current_time(MILLISECOND) - philo->data->start_time;
    pthread_mutex_lock(&philo->data->print_mutex);
    if ((status == TAKE_FIRST_FORK || status == TAKE_SECOND_FORK) && !simulation_finished(philo->data))
        printf("%ld %d has taken a fork\n", current_time, philo->id); 
    else if (status == EATING && !simulation_finished(philo->data))
        printf("%ld %d is eating\n", current_time, philo->id);
    else if (status == SLEEPING && !simulation_finished(philo->data))
        printf("%ld %d is sleeping\n", current_time, philo->id);
    else if (status == THINKING && !simulation_finished(philo->data))
        printf("%ld %d is thinking\n", current_time, philo->id);
    else if (status == DEAD)
        printf("%ld %d died\n", current_time, philo->id);
    pthread_mutex_unlock(&philo->data->print_mutex);
}