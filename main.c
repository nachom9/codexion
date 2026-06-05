#include "codexion.h"

int compile(t_coder *coder)
{
    if (coder->params->state == 0)
        return (0);
    printf("%ld %d is compiling\n", get_time() - coder->params->start_time, coder->id);
    coder->last_compile = get_time() - coder->params->start_time;
    usleep(coder->params->time_to_compile * 1000);
    if (coder->params->state == 0)
        return (0);
    printf("%ld %d is debugging\n", get_time() - coder->params->start_time, coder->id);
    usleep(coder->params->time_to_debug * 1000);
    if (coder->params->state == 0)
        return (0);
    printf("%ld %d is refactoring\n", get_time() - coder->params->start_time, coder->id);
    usleep(coder->params->time_to_refactor * 1000);
    coder->compiles -= 1;
    return (0);
}

/*
void    lock_dongles(int i, int j, pthread_t *coder)
{
    pthread_mutex_lock(&coder->params->dongles->dongles[i]->mutex);
    pthread_mutex_lock(&coder->params->dongles->dongles[j]->mutex);
}*/

int    take_dongles(t_coder *coder)
{
    int left;
    int right;

    right = coder->id - 1;
    left = coder->id % coder->params->number_of_coders;


    if (coder->id % 2 == 0)
    {
        //printf("coder %d locked dongle %d\n", coder->id, right);
        pthread_mutex_lock(&coder->params->dongles->dongles[right]->mutex);
        if (coder->params->state == 0)
        {
            pthread_mutex_unlock(&coder->params->dongles->dongles[right]->mutex);
            return (0);
        }
        //printf("coder %d locked dongle %d\n", coder->id, left);
        pthread_mutex_lock(&coder->params->dongles->dongles[left]->mutex);
    }
    else
    {
        //printf("coder %d locked dongle %d\n", coder->id, left);
        pthread_mutex_lock(&coder->params->dongles->dongles[left]->mutex);
        if (coder->params->state == 1)
        {
            //printf("coder %d locked dongle %d\n", coder->id, right);
            pthread_mutex_lock(&coder->params->dongles->dongles[right]->mutex);
        }

    }
    if (coder->params->state == 0)
    {
        //printf("coder %d unlocked dongle %d\n", coder->id, left);
        pthread_mutex_unlock(&coder->params->dongles->dongles[left]->mutex);
        //printf("coder %d unlocked dongle %d\n", coder->id, right);
        pthread_mutex_unlock(&coder->params->dongles->dongles[right]->mutex);
        return (0);
    }
    printf("%ld %d has taken a dongle\n", get_time() - coder->params->start_time, coder->id);
    printf("%ld %d has taken a dongle\n", get_time() - coder->params->start_time, coder->id);
    compile(coder);
    if (coder->params->state == 1)
        usleep(coder->params->dongle_cooldown * 1000);
    //printf("coder %d unlocked dongle %d\n", coder->id, left);
    pthread_mutex_unlock(&coder->params->dongles->dongles[left]->mutex);
    //printf("coder %d unlocked dongle %d\n", coder->id, right);
    pthread_mutex_unlock(&coder->params->dongles->dongles[right]->mutex);
    return (0);
}

void    *coder_killer(void *arg)
{
    int i;
    int j;
    t_params   *params;
    t_coder *coder;

    j = 0;
    params = (t_params *)arg;
    while(j < params->number_of_coders)
    {
        i = 0;
        j = 0;
        while (i < params->number_of_coders)
        {
            coder = params->coders->coders[i++];
            if (coder->compiles == 0)
                j++;
            if ((get_time() - params->start_time) - coder->last_compile > params->time_to_burnout)
            {
                params->state = 0;
                printf("%ld %d burned out\n", get_time() - params->start_time, coder->id);
                return (NULL);
            }
        }
        usleep(100);
    }
    return (NULL);
}

int ft_min(t_coder *coder)
{
    int i;

    i = 0;
    while (i < coder->params->number_of_coders)
    {
        if (coder->params->coders->coders[i]->last_compile < coder->last_compile)
            return (0);
        i++;
    }
    return (1);
}

int coder_prio(t_coder *coder)
{
    long right;
    long left;

    if (coder->id == 0)
    {
        right = coder->params->coders->coders[1]->last_compile;
        left = coder->params->coders->coders[coder->params->number_of_coders]->last_compile;
    }
    else if (coder->id == coder->params->number_of_coders)
    {
        right = coder->params->coders->coders[0]->last_compile;
        left = coder->params->coders->coders[coder->id - 2]->last_compile;
    }
    else
    {
        right = coder->params->coders->coders[coder->id]->last_compile;
        left = coder->params->coders->coders[coder->id - 2]->last_compile;
    }
    //printf("%ld\n", coder->last_compile);
    if (coder->last_compile <= right && coder->last_compile <= left)
        return 1;
    else
        return 0;
}

void    *print_state(void *arg)
{
    t_coder *coder;

    coder = (t_coder *)arg;
    while(coder->compiles > 0 && coder->params->state == 1)
    {
        if (ft_min(coder))
            take_dongles(coder);
        //printf("coder id: %d | coder->compiles: %d | state: %d\n", coder->id, coder->compiles, coder->params->state);
        usleep(500);
    }
    return (NULL);
}

void    *alg_fifo(void *arg)
{
    t_coder *coder;

    coder = (t_coder *)arg;
    while(coder->compiles > 0 && coder->params->state == 1)
    {
            take_dongles(coder);
    }
    return (NULL);
}

void    *alg_edf(void *arg)
{
    t_coder *coder;

    coder = (t_coder *)arg;
    while(coder->compiles > 0 && coder->params->state == 1)
    {
        if (ft_min(coder))
            take_dongles(coder);
    }
    return (NULL);
}

long    get_time(void)
{
    struct timeval  tv;

    gettimeofday(&tv, NULL);
    return (tv.tv_sec * 1000L + tv.tv_usec / 1000);
}

int main(int argc, char *argv[])
{
    t_params    *params;
    pthread_t death_thread;
    t_coders *coders;
    t_dongles   *dongles;
    long    start_time;

    int i = 0;
    start_time = get_time();
    if (argc != 9)
    {
        printf("Arguments missing!\n");
        return (0);
    }
    params = parse_params(argv, start_time);
    dongles = set_dongles(params->number_of_coders);
    params->dongles = dongles;
    coders = set_coders(params);
    params->coders = coders;
    pthread_create(&death_thread, NULL, coder_killer, params);
    if (params->number_of_coders > 1)
    {
        while (i < params->number_of_coders)
        {
            pthread_create(&coders->coders[i]->thread, NULL, print_state, coders->coders[i]);
            i++;
        }
        i = 0;
        pthread_join(death_thread, NULL);
        while (i < params->number_of_coders)
        {
            pthread_join(coders->coders[i]->thread, NULL);
            free(coders->coders[i]);
            free(dongles->dongles[i]);
            //printf("%d\n", i);
            i++;
        }
    }
    if (params->number_of_coders == 1)
    {
        free(coders->coders[i]);
        free(dongles->dongles[i]);
    }
    free(coders->coders);
    free(dongles->dongles);
    free(coders);
    free(dongles);
    free(params);
    return (0);
}
