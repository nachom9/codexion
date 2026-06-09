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
    int compiles;
    long last_compile;
    pthread_t thread;
    t_params *params;
}	t_coder;

typedef struct s_dongle
{
    int id;
    pthread_mutex_t mutex;
}	t_dongle;

typedef struct s_coders
{
    t_coder **coders;
}	t_coders;

typedef struct s_dongles
{
    t_dongle **dongles;
}	t_dongles;

typedef struct s_params
{
	int number_of_coders;
    long time_to_burnout;
    int time_to_compile;
    int time_to_debug;
    int time_to_refactor;
    int compiles_required;
    int dongle_cooldown;
    int state;
    char    *scheduler;
    pthread_mutex_t mutex;
    t_coders *coders;
    t_dongles *dongles;
    long start_time;
}	t_params;

void    *ft_edf(void *arg);
void    *ft_fifo(void *arg);
t_params    *parse_params(char **args, long start_time);
t_dongles *set_dongles(int nb_of_coders);
t_coders *set_coders(t_params *params);
t_coder	*create_coder(int id, t_params *params);
t_dongle	*create_dongle(int id);
void    *alg_fifo(void *arg);
void    *alg_edf(void *arg);
int    take_dongles(t_coder *coder);
int compile(t_coder *coder);
long    get_time(void);
void    *coder_killer(void *arg);
int ft_min(t_coder *coder);


#endif