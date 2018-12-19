# include <stdio.h>
# include <pthread.h>
#include <stdlib.h>

# define BufferSize 6
# define P_NUM_OF_PRODUCTS 40
# define C_NUM_OF_PRODUCTS 30
# define LIMIT 1000000

void *Producer();
void *Consumer();

int BufferIndex=-1;
int *BUFFER;

pthread_cond_t Buffer_Not_Full=PTHREAD_COND_INITIALIZER;
pthread_cond_t Buffer_Not_Empty=PTHREAD_COND_INITIALIZER;

pthread_mutex_t mVar_ALL=PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mVar_c_check=PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mVar_p_check=PTHREAD_MUTEX_INITIALIZER;

int main()
{

    pthread_t ptid_A, ptid_B, ptid_C, ctid_A, ctid_B, ctid_C, ctid_D;

    BUFFER = (int *) malloc (BufferSize * sizeof(int));

    pthread_create(&ptid_A,NULL,Producer,NULL);
    pthread_create(&ptid_B,NULL,Producer,NULL);
    pthread_create(&ptid_C,NULL,Producer,NULL);

    pthread_create(&ctid_A,NULL,Consumer,NULL);
    pthread_create(&ctid_B,NULL,Consumer,NULL);
    pthread_create(&ctid_C,NULL,Consumer,NULL);
    pthread_create(&ctid_D,NULL,Consumer,NULL);

    pthread_join(ptid_A,NULL);
    pthread_join(ptid_B,NULL);
    pthread_join(ptid_C,NULL);

    pthread_join(ctid_A,NULL);
    pthread_join(ctid_B,NULL);
    pthread_join(ctid_C,NULL);
    pthread_join(ctid_D,NULL);

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
