#include "codexion.h"

t_dongles *set_dongles(int nb_of_coders)
{
    int i;
    t_dongles    *dongles;

    i = 1;
    while (i <= nb_of_coders)
    {
        dongles->dongles[i] = create_dongle(i);
        i++;
    }
    return (dongles);
}

t_coders *set_coders(int nb_of_coders)
{
    int i;
    t_coders    *coders;

    i = 1;
    while (i <= nb_of_coders)
    {
        coders->coders[i] = create_coder(i);
        i++;
    }
    return (coders);
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

    if (argc != 9)
    {
        printf("Arguments missing!");
        return (0);
    }
    params = parse_params(argv);
    coders = set_coders(params->number_of_coders);
    dongles = set_dongles(params->number_of_coders);
    return (0);
}
