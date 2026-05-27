#include "codexion.h"
int number = 0;
void    *ft_print(pthread_mutex_t *mutex)
{
    while (1)
    {
    pthread_mutex_lock(mutex);
    printf("%d\n", number);
    number++;
    pthread_mutex_unlock(mutex);
    usleep(300000);
    }
    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    pthread_t   th;
    t_coder *head;
    t_coder *coder;
    int result;
    int nb_of_coders;
    int i;
    pthread_mutex_t mutex;

    if (argc < 2)
    {
        exit(1);
    }
    i = 0;
    nb_of_coders = atoi(argv[1]);
    while (nb_of_coders > 0)
    {
        result = pthread_create(&th, NULL, ft_print, &mutex);
        if (result != 0)
        {
            perror("Error creating thread.\n");
            exit(EXIT_FAILURE);
        }
        coder = ft_lstnew(&th, i);
        i++;
        nb_of_coders--;
    }

    pthread_join(th, NULL);
    printf("End of program.\n");
    return (0);
}