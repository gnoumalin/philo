#include "philo.h"

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

long	get_current_time(t_time_code code)
{
	struct timeval	time;

	if (gettimeofday(&time, NULL) == -1)
		write(2, "gettimeofday() error\n", 22);
	if (code == SECOND)
		return (time.tv_sec + (time.tv_usec / 1e6));
	else if (code == MILLISECOND)
		return ((time.tv_sec * 1e3) + (time.tv_usec / 1e3));
	else if (code == MICROSECOND)
		return ((time.tv_sec * 1e6) + time.tv_usec);
	else 
		write(2, "Invalid time code\n", 18);
	return (1);
}

void precise_sleep(long time ,t_data *data)
{
    long start_time;
    long elapsed_time;
    long remaining_time;

    start_time = get_current_time(MICROSECOND);
    while (get_current_time(MICROSECOND) - start_time < time)
    {
        if (simulation_finished(data))
            break ;
        elapsed_time = get_current_time(MICROSECOND) - start_time;
        remaining_time = time - elapsed_time;
        if (remaining_time > 1e4)
            usleep(remaining_time / 2);
        else
        {
            while (get_current_time(MICROSECOND) - start_time < time)
                ;
        }
    }
}
