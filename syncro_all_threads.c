#include "philo.h"

void wait_all_threads(t_data *data)
{
    while (!get_bool(&data->data_mutex, &data->sync))
        ;
}