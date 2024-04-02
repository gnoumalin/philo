#include "philo.h"

bool all_threads_active(t_data *data)
{
	return (get_long(&data->data_mutex, &data->philo_ready) == get_long(&data->data_mutex, &data->philo_nbr));
}

void *monitor_routine(void *arg)
{
    t_data *data;
    int i;
	int j;

	j = 0;
    data = (t_data *)arg;
    while (!all_threads_active(data))
		;
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