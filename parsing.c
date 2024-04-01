#include "philo.h"

int init_parsing(int argc, char **argv)
{
    if (argc != 5 && argc != 6)
	{
		printf("Error: wrong number of arguments\n");
		return (1);
	}
    if (check_numeric(argc, argv) == 1)
        return (1);
    if (check_max_values(argc, argv) == 1)
        return (1);
    if (check_negative(argc, argv) == 1)
        return (1);
    return (0);
}

int check_max_values(int argc, char **argv)
{
    int i;
    int j;

    i = 1;
    while (i < argc)
    {
        if (ft_atol(argv[i]) > 4294967295)
        {
            printf("Error: argument %d is too big\n", i);
            return (1);
        }
        i++;
    }
    return (0);
}

bool ft_isdigit(int c)
{
    if (c >= '0' && c <= '9')
        return (true);
    return (false);
}

int check_numeric(int argc, char **argv)
{
    int i;
    int j;

    i = 1;
    while (i < argc)
    {
        j = 0;
        while (argv[i][j])
        {
            if (!ft_isdigit(argv[i][j]))
            {
                printf("Error: argument %d is not numeric\n", i);
                return (1);
            }
            j++;
        }
        i++;
    }
    return (0);
}

int check_negative(int argc, char **argv)
{
    int i;
    int j;

    i = 1;
    while (i < argc)
    {
        j = 0;
        if (argv[i][j] == '-')
        {
            printf("Error: argument %d is negative\n", i);
            return (1);
        }
        i++;
    }
    return (0);
}