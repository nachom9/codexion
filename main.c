#include "codexion.h"

int compile(t_coder *coder)
{
    usleep(coder->params->time_to_compile * 1000);
    printf("%ld %d is compiling\n", get_time() - coder->params->start_time, coder->id);
    usleep(coder->params->time_to_debug * 1000);
    printf("%ld %d is debugging\n", get_time() - coder->params->start_time, coder->id);
    usleep(coder->params->time_to_refactor * 1000);
    printf("%ld %d is refactoring\n", get_time() - coder->params->start_time, coder->id);
    coder->compiles -= 1;
    coder->last_compile = get_time() - coder->params->start_time;
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

    printf("Coder %d took dongles %d and %d.\n", coder->id,
        coder->params->dongles->dongles[left]->id,
        coder->params->dongles->dongles[right]->id);
    compile(coder);
    pthread_mutex_unlock(&coder->params->dongles->dongles[left]->mutex);
    pthread_mutex_unlock(&coder->params->dongles->dongles[right]->mutex);
    return (0);
}

void    *coder_killer(void *arg)
{
    int alive;
    int i;
    t_params   *params;
    t_coder *coder;

    params = (t_params *)arg;
    alive = 1;

    while(alive != 0)
    {
        i = 0;
        while (i < params->number_of_coders)
        {
            coder = params->coders->coders[i];
            if (coder->state == 0)
                continue;
            if (coder->state == 1)
                alive = 0;
            if (get_time() - coder->last_compile > params->time_to_burnout + get_time() - params->start_time)
            {
                coder->state = 0;
                while (i < params->number_of_coders)
                {
                    params->coders->coders[i]->state == 0;
                    i++;
                }
                printf("%ld %d burned out\n", get_time() - params->start_time, coder->id);
                return (NULL);
            }
            i++;
        }
        usleep(500);
    }
    return (NULL);
}

void    *print_state(void *arg)
{
    t_coder *coder;

    coder = (t_coder *)arg;
    while(coder->compiles > 0 && coder->state == 1)
    {
        take_dongles(coder);
        usleep(coder->params->dongle_cooldown * 1000);
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
