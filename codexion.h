#ifndef CODEXION_H
# define CODEXION_H

# include <stdlib.h>
# include <pthread.h>
# include <unistd.h>
# include <stdio.h>
# include <string.h>
# include <sys/time.h>

typedef struct s_coder
{
    int id;
    t_params *params;
}	t_coder;

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
    t_coder *coders;
}	t_params;

typedef struct s_

t_coder	*ft_lstnew(void *content, int id);

#endif