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
int    take_dongles(t_coder *coder)
{
    int left;
    int right;

    right = coder->id - 1;
    left = coder->id % coder->params->number_of_coders;

    if (coder->id % 2 == 0)
    {
        pthread_mutex_lock(&coder->params->dongles->dongles[right]->mutex);
        pthread_mutex_lock(&coder->params->dongles->dongles[left]->mutex);
    }
    else
    {
        pthread_mutex_lock(&coder->params->dongles->dongles[left]->mutex);
        pthread_mutex_lock(&coder->params->dongles->dongles[right]->mutex);
    }
    if (coder->params->state == 0)
    {
        pthread_mutex_unlock(&coder->params->dongles->dongles[left]->mutex);
        pthread_mutex_unlock(&coder->params->dongles->dongles[right]->mutex);
        return (0);
    }
    printf("%ld %d has taken a dongle\n", get_time() - coder->params->start_time, coder->id);
    printf("%ld %d has taken a dongle\n", get_time() - coder->params->start_time, coder->id);
    compile(coder);
    usleep(coder->params->dongle_cooldown * 1000);
    pthread_mutex_unlock(&coder->params->dongles->dongles[left]->mutex);
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
        while (i < params->number_of_coders)
        {
            coder = params->coders->coders[i];
            //aquí la j sigue sumando al mismo programador que haya acabado, pendiente de arreglar
            if (!coder->compiles)
                j++;
            if ((get_time() - params->start_time) - coder->last_compile > params->time_to_burnout)
            {
                params->state = 0;
                printf("%ld %d burned out\n", get_time() - params->start_time, coder->id);
                return (NULL);
            }
            i++;
        }
        usleep(100);
    }
    return (NULL);
}

void    *print_state(void *arg)
{
    t_coder *coder;

    coder = (t_coder *)arg;
    while(coder->compiles > 0 && coder->params->state == 1)
    {
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
    while (i < params->number_of_coders)
    {
        pthread_create(&coders->coders[i]->thread, NULL, print_state, coders->coders[i]);
        i++;
    }
    i = 0;
    while (i < params->number_of_coders)
    {
        pthread_join(coders->coders[i]->thread, NULL);
        free(coders->coders[i]);
        free(dongles->dongles[i]);
        i++;
    }
    pthread_join(death_thread, NULL);
    free(coders->coders);
    free(dongles->dongles);
    free(coders);
    free(dongles);
    free(params);
    return (0);
}
