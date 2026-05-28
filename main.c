#include "codexion.h"

t_dongles *set_dongles(int nb_of_coders)
{
    int i;
    t_dongles    *all_dongles;

    i = 1;
    all_dongles = malloc(sizeof(t_dongles));
    if (!all_dongles)
        return (NULL);
    all_dongles->dongles = malloc(sizeof(t_dongle *) * nb_of_coders);
    if (!all_dongles->dongles)
        return (NULL);
    while (i <= nb_of_coders)
    {
        all_dongles->dongles[i] = create_dongle(i);
        i++;
    }
    return (all_dongles);
}

void    *print_state(void *arg)
{
    t_coder *coder;

    coder = (t_coder *)arg;
    while(1)
    {
        pthread_mutex_lock(&coder->mutex);
        printf("%d\n", coder->id);
        pthread_mutex_unlock(&coder->mutex);
    }
    return (NULL);
}

t_coders *set_coders(t_params *params)
{
    int i;
    t_coders    *all_coders;
    t_coder *coder;
    
    i = 0;
    all_coders = malloc(sizeof(t_coders));
    if (!all_coders)
        return (NULL);
    all_coders->coders = malloc(sizeof(t_coder *) * params->number_of_coders);
    if (!all_coders->coders)
        return (NULL);

    if (!all_coders)
        return (NULL);
    while (i < params->number_of_coders)
    {
        coder = create_coder(i + 1);
        if (!coder)
            return (NULL);
        pthread_create(&coder->thread, NULL, print_state, coder);
        all_coders->coders[i] = coder;
        i++;
    }
    return (all_coders);
}

t_params    *parse_params(char **args)
{
    t_params	*node;

	node = malloc(sizeof(t_params));
	if (!node)
		return (NULL);
    node->number_of_coders = atoi(args[1]);
    node->time_to_burnout = atoi(args[2]);
    node->time_to_compile = atoi(args[3]);
    node->time_to_debug = atoi(args[4]);
    node->time_to_refactor = atoi(args[5]);
    node->compiles_required = atoi(args[6]);
    node->dongle_cooldown = atoi(args[7]);
    node->scheduler = args[8];
    return(node);
}

int main(int argc, char *argv[])
{
    t_params    *params;
    t_coders *coders;
    t_dongles   *dongles;
    int i = 0;

    if (argc != 9)
    {
        printf("Arguments missing!");
        return (0);
    }
    params = parse_params(argv);
    coders = set_coders(params);
    dongles = set_dongles(params->number_of_coders);
    params->dongles = dongles;
    params->coders = coders;
    while (i < params->number_of_coders)
    {
        pthread_join(coders->coders[i]->thread, NULL);
        i++;
    }

    return (0);
}
