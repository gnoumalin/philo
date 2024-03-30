#include "philo.h"

void *monitor_routine(void *arg)
{
    t_data *data;
    int i;

    data = (t_data *)arg;
    while (!simulation_finished(data))
    {
        i = 0;
        while (i < data->philo_nbr && !simulation_finished(data))
        {
            if (is_philo_dead(&data->philos[i]))
            {
                set_bool(&data->data_mutex, &data->end, true);
                write_status(DEAD, &data->philos[i]);
            }
            i++;
        }
    }
    return (NULL);
}