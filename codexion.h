#ifndef CODEXION_H
# define CODEXION_H

# include <stdlib.h>
# include <pthread.h>
# include <unistd.h>
# include <stdio.h>
# include <string.h>
# include <sys/time.h>

typedef struct s_params	t_params;

typedef struct s_coder
{
	int				id;
	int				compiles;
	long			last_compile;
	pthread_mutex_t	mutex;
	pthread_t		thread;
	t_params		*params;
}	t_coder;

typedef struct s_dongle
{
	int				id;
	pthread_mutex_t	mutex;
}	t_dongle;

typedef struct s_coders
{
	t_coder	**coders;
}	t_coders;

typedef struct s_dongles
{
	t_dongle	**dongles;
}	t_dongles;

typedef struct s_params
{
	int				number_of_coders;
	long			time_to_burnout;
	int				time_to_compile;
	int				time_to_debug;
	int				time_to_refactor;
	int				compiles_required;
	int				dongle_cooldown;
	int				state;
	char			*scheduler;
	pthread_mutex_t	mutex;
	t_coders		*coders;
	t_dongles		*dongles;
	long			start_time;
	pthread_t		death_thread;
}	t_params;

int			check_scheduler(char *scheduler);
int			check_state(t_params *params);
void		end_state(t_params *params);
void		*ft_edf(void *arg);
void		*ft_fifo(void *arg);
t_params	*parse_params(char **args);
t_dongles	*set_dongles(int nb_of_coders);
t_coders	*set_coders(t_params *params);
t_coder		*create_coder(int id, t_params *params);
t_dongle	*create_dongle(int id);
int			take_dongles(t_coder *coder);
int			compile(t_coder *coder);
long		get_time(void);
void		*set_death_algorithm(void *arg);
void		*death_algorithm(t_params *params, t_coder *coder, int i, int j);
int			ft_min(t_coder *coder);
void		set_threads(t_params *params, t_coders *coders, t_dongles *dongles);
void		free_all(t_params *params);
int			take_dongles_even(t_coder *coder, int right, int left);
int			take_dongles_odd(t_coder *coder, int right, int left);

#endif
