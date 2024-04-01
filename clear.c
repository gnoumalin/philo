#include "philo.h"

void clear_data(t_data *data)
{
    int i;

    i = 0;
    while (i < data->philo_nbr)
    {
        pthread_mutex_destroy(&data->philos[i].mutex);
        i++;
    }
    pthread_mutex_destroy(&data->data_mutex);
    pthread_mutex_destroy(&data->print_mutex);
    free(data->philos);
    free(data->forks);
}