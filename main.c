#include "codexion.h"

int compile(t_coder *coder)
{
    if (check_state(coder->params) == 0)
        return (0);
    printf("%ld %d is compiling\n", get_time() - coder->params->start_time, coder->id);
    coder->last_compile = get_time() - coder->params->start_time;
    usleep(coder->params->time_to_compile * 1000);
    if (check_state(coder->params) == 0)
        return (0);
    printf("%ld %d is debugging\n", get_time() - coder->params->start_time, coder->id);
    usleep(coder->params->time_to_debug * 1000);
    if (check_state(coder->params) == 0)
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
    int state;

    right = coder->id - 1;
    left = coder->id % coder->params->number_of_coders;


    if (coder->id % 2 == 0)
    {
        pthread_mutex_lock(&coder->params->dongles->dongles[right]->mutex);
        if (check_state(coder->params) == 0)
        {
            pthread_mutex_unlock(&coder->params->dongles->dongles[right]->mutex);
            return (0);
        }
        pthread_mutex_lock(&coder->params->dongles->dongles[left]->mutex);
    }
    else
    {
        pthread_mutex_lock(&coder->params->dongles->dongles[left]->mutex);
        if (check_state(coder->params) == 0)
        {
            pthread_mutex_unlock(&coder->params->dongles->dongles[left]->mutex);
            return (0);
        }
        pthread_mutex_lock(&coder->params->dongles->dongles[right]->mutex);
    }
    if (check_state(coder->params) == 0)
    {
        pthread_mutex_unlock(&coder->params->dongles->dongles[left]->mutex);
        pthread_mutex_unlock(&coder->params->dongles->dongles[right]->mutex);
        return (0);
    }
    printf("%ld %d has taken a dongle\n", get_time() - coder->params->start_time, coder->id);
    printf("%ld %d has taken a dongle\n", get_time() - coder->params->start_time, coder->id);
    compile(coder);
    if (check_state(coder->params) == 1)
        usleep(coder->params->dongle_cooldown * 1000);
    pthread_mutex_unlock(&coder->params->dongles->dongles[left]->mutex);
    pthread_mutex_unlock(&coder->params->dongles->dongles[right]->mutex);
    return (0);
}

void    set_threads(t_params *params)
{

}

int main(int argc, char *argv[])
{
    t_params    *params;
    pthread_t death_thread;
    t_coders *coders;
    t_dongles   *dongles;

    int i = 0;
    if (argc != 9)
    {
        printf("Invalid arguments\n");
        return (0);
    }
    params = parse_params(argv);
    dongles = set_dongles(params->number_of_coders);
    params->dongles = dongles;
    coders = set_coders(params);
    params->coders = coders;
    set_threads(params);
    pthread_create(&death_thread, NULL, set_death_algorithm, params);
    if (params->number_of_coders > 1)
    {
        while (i < params->number_of_coders)
        {
            if (check_scheduler(params->scheduler) == 1)
                pthread_create(&coders->coders[i]->thread, NULL, ft_fifo, coders->coders[i]);
            if (check_scheduler(params->scheduler) == 2)
                pthread_create(&coders->coders[i]->thread, NULL, ft_edf, coders->coders[i]);
            i++;
        }
        i = 0;
        pthread_join(death_thread, NULL);
        while (i < params->number_of_coders)
        {
            pthread_join(coders->coders[i]->thread, NULL);
            free(coders->coders[i]);
            free(dongles->dongles[i]);
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
