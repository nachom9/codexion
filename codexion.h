#ifndef CODEXION_H
# define CODEXION_H

# include <stdlib.h>
# include <pthread.h>
# include <unistd.h>
# include <stdio.h>
# include <string.h>
# include <sys/time.h>

typedef struct s_params t_params;

typedef struct s_coder
{
    int id;
}	t_coder;

typedef struct s_dongle
{
    int id;
}	t_dongle;

typedef struct s_coders
{
    t_coder **coders;
    t_params *params;
}	t_coders;

typedef struct s_dongles
{
    t_dongle **dongles;
    t_params *params;
}	t_dongles;

typedef struct s_params
{
	int number_of_coders;
    int time_to_burnout;
    int time_to_compile;
    int time_to_debug;
    int time_to_refactor;
    int compiles_required;
    int dongle_cooldown;
    char    *scheduler;
    t_coder **coders;
    t_dongle **dongles;
}	t_params;


t_coder	*create_coder(int id);
t_dongle	*create_dongle(int id);

#endif