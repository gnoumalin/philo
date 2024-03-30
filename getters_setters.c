#include "philo.h"

void    set_bool(pthread_mutex_t *mutex, bool *var, bool value)
{
    pthread_mutex_lock(mutex);
    *var = value;
    pthread_mutex_unlock(mutex);
}

bool    get_bool(pthread_mutex_t *mutex, bool *var)
{
    bool    value;

    pthread_mutex_lock(mutex);
    value = *var;
    pthread_mutex_unlock(mutex);
    return (value);
}

void    set_long(pthread_mutex_t *mutex, long *var, long value)
{
    pthread_mutex_lock(mutex);
    *var = value;
    pthread_mutex_unlock(mutex);
}

long    get_long(pthread_mutex_t *mutex, long *var)
{
    long    value;

    pthread_mutex_lock(mutex);
    value = *var;
    pthread_mutex_unlock(mutex);
    return (value);
}

bool   is_philo_dead(t_philo *philo)
{
    long    last_meal_time;

    if (get_bool(&philo->mutex, &philo->full))
        return (false);
    last_meal_time = get_current_time(MILLISECOND) - get_long(&philo->mutex, &philo->last_meal);
    if (last_meal_time > philo->data->time_to_die / 1e3)
        return (true);
    return (false);
}

bool simulation_finished(t_data *data)
{
    return (get_bool(&data->data_mutex, &data->end)); 
}