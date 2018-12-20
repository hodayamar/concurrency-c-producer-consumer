# include <stdio.h>
# include <pthread.h>
#include <stdlib.h>

# define BufferSize 6
# define P_NUM_OF_PRODUCTS 40
# define C_NUM_OF_PRODUCTS 30
# define LIMIT 1000000
# define NUM_OF_PRUD 3
# define NUM_OF_CON 4

void *Producer();
void *Consumer();

int BufferIndex = -1;
int *BUFFER;

pthread_cond_t Buffer_Not_Full = PTHREAD_COND_INITIALIZER;
pthread_cond_t Buffer_Not_Empty = PTHREAD_COND_INITIALIZER;

pthread_mutex_t mVar_ALL = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mVar_c_check = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mVar_p_check = PTHREAD_MUTEX_INITIALIZER;

int main()
{

    int i;
    pthread_t thraeds [7];

    BUFFER = (int *) malloc (BufferSize * sizeof(int));

    for (i = 0; i < NUM_OF_PRUD; i++)

        pthread_create(&thraeds[i],NULL,Producer,NULL);


    for (i = NUM_OF_PRUD ; i < NUM_OF_CON + NUM_OF_PRUD; i++)

        pthread_create(&thraeds[i],NULL,Consumer,NULL);

    for (i = 0; i < NUM_OF_PRUD + NUM_OF_CON; i++)

        pthread_join(thraeds[i],NULL);

    return 0;
}

int rand_num()
{
    return (rand() % LIMIT);
}

void *Producer()
{
    int i, prdct;

    for(i = 0; i < P_NUM_OF_PRODUCTS; i++)
    {
        pthread_mutex_lock(&mVar_p_check);
        pthread_mutex_lock(&mVar_ALL);

        if(BufferIndex == BufferSize - 1)

            pthread_cond_wait(&Buffer_Not_Full, &mVar_ALL);

        pthread_mutex_unlock(&mVar_p_check);

        prdct = rand_num();
        BUFFER[++BufferIndex] = prdct;
        printf("insert to: %d element is -- %d\n", BufferIndex, prdct);

        pthread_mutex_unlock(&mVar_ALL);
        pthread_cond_signal(&Buffer_Not_Empty);
    }

    return  0;
}

void *Consumer()
{
    int i;
    for(i = 0; i < C_NUM_OF_PRODUCTS; i++)
    {
        pthread_mutex_lock(&mVar_c_check);
        pthread_mutex_lock(&mVar_ALL);

        if(BufferIndex == -1)

            pthread_cond_wait(&Buffer_Not_Empty,&mVar_ALL);

        pthread_mutex_unlock(&mVar_c_check);

        printf("extract element --- %d from index --- %d\n", BUFFER[BufferIndex], BufferIndex);

        BufferIndex--;

        pthread_mutex_unlock(&mVar_ALL);
        pthread_cond_signal(&Buffer_Not_Full);
    }

    return  0;
}
