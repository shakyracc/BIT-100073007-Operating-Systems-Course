#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h> // for sleep function in linux
#include <string.h>
#include <semaphore.h>

#define BUFFER_SIZE 4

#define PRODUCERS 3
#define CONSUMERS 4

#define PRODUCER_ITERATIONS 4 // number of times producer loops
#define CONSUMER_ITERATIONS 3

pthread_t consumerId[CONSUMERS], producerId[PRODUCERS];

sem_t empty, full, mutex;

typedef struct
{
    int value[BUFFER_SIZE];
    int next_in, next_out;
} buffer_t;

buffer_t buffer;

int insertInitial(char initial, long int id)
{
    printf(" next_in = %d next_out = %d \n", buffer.next_in, buffer.next_out);

    buffer.value[buffer.next_in] = initial;
    buffer.next_in = (buffer.next_in + 1) % BUFFER_SIZE;
    printf("producer %ld: inserted %c\n", id, initial);

    printf("Buffer: ");
    for (int i = 0; i < BUFFER_SIZE; i++)
    {
        printf("%c ", buffer.value[i]);
    }
    printf("\n");

    return 0;
}

void *Producer(void *param)
{
    printf("\n\tProducer %ld created and will iterate %d times\n", (long int)param + 1, PRODUCER_ITERATIONS);

    char initials[] = "XQL";
    int length = strlen(initials);

    printf("length: %d\n", length);

    char initial;
    long int id = (long int)param;

    int j = PRODUCER_ITERATIONS;
    while (j--)
    {

        // wait for random length of time from 0 to 3 seconds
        int randnum = rand() % 4; // range between 0 and 3
        printf("\t\tWaiting for %d seconds in iteration %d of Producer %ld\n", randnum, j, (long int)param + 1);
        sleep(randnum);

        // insert random initial into buffer

        // int item;
        // long int id = (long int)param;

        int randinitial = rand() % length;
        printf("randinitial = %d\n", randinitial);
        printf("Random initial: %c\n", initials[randinitial]);

        sem_wait(&empty);
        sem_wait(&mutex);
        initial = initials[randinitial];
        printf("initial: %c\n", initial);
        if (insertInitial(initial, id))
            fprintf(stderr, "Error while inserting to buffer\n");

        sem_post(&mutex);
        sem_post(&full);
        // item = rand() % 10000;
        // if (insert_item(item, id))
        //     fprintf(stderr, "Error while inserting to buffer\n");
    }

    pthread_exit(0);
}
void *Consumer(void *param)
{
    printf("\n\tConsumer %ld created and will iterate %d times\n", (long int)param + 1, CONSUMER_ITERATIONS);

    pthread_exit(0);
}

int main()
{

    buffer.next_in = 0;
    buffer.next_out = 0;

    // Initialize buffer with '-'

    printf("\nInitialize buffer of size %d with '-' \n", BUFFER_SIZE);

    printf("Buffer: ");
    for (int i = 0; i < BUFFER_SIZE; i++)
    {
        buffer.value[i] = '-';
        printf("%c ", buffer.value[i]);
    }
    printf("\n");

    // create producer and consumer threads

    long int i;

    // create consumer threads
    printf("\nCreating %d Consumers and %d Producers\n", CONSUMERS, PRODUCERS);

    sem_init(&full, 0, 0);
    sem_init(&empty, 0, BUFFER_SIZE);
    sem_init(&mutex, 0, 1);

    for (i = 0; i < CONSUMERS; i++)
        if (pthread_create(&consumerId[i], NULL, Consumer, (void *)i) != 0)
        {
            perror("pthread_create");
            abort();
        }
    // Create the producer threads
    for (i = 0; i < PRODUCERS; i++)
        if (pthread_create(&producerId[i], NULL, Producer, (void *)i) != 0)
        {
            perror("pthread_create");
            abort();
        }

    // Wait for threads to complete
    for (i = 0; i < CONSUMERS; i++)
        if (pthread_join(consumerId[i], NULL) != 0)
        {
            perror("pthread_join");
            abort();
        }
    for (i = 0; i < PRODUCERS; i++)
        if (pthread_join(producerId[i], NULL) != 0)
        {
            perror("pthread_join");
            abort();
        }
}