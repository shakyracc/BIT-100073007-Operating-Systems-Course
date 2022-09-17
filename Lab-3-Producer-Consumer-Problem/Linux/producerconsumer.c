// BIT 100073007 Operating Systems Course Lab 3: Producer-Consumer Problem

#include <stdio.h> // printf(), fprintf()
#include <stdlib.h> // rand() 
#include <pthread.h> // pthread_...
#include <unistd.h> // sleep()
#include <string.h> // strlen()
#include <semaphore.h> // sem_...

#define BUFFER_SIZE 4

#define PRODUCERS 3
#define CONSUMERS 4

#define PRODUCER_ITERATIONS 4 // number of times producer loops
#define CONSUMER_ITERATIONS 3 // number of times consumer loops 

pthread_t consumerId[CONSUMERS], producerId[PRODUCERS]; //define threads

sem_t empty, full, mutex; //define 3 semaphores 

typedef struct
{
    char value[BUFFER_SIZE];
    int next_in, next_out;
} buffer_t;

buffer_t buffer;


// insertInitial function is used to add an initial to the buffer
int insertInitial(char initial, long int id)
{

    buffer.value[buffer.next_in] = initial;
    buffer.next_in = (buffer.next_in + 1) % BUFFER_SIZE;
    printf("producer %ld: produced %c\n", id, initial);

    printf("\t\t\t\t\t\tBuffer: ");
    for (int i = 0; i < BUFFER_SIZE; i++)
    {
        printf("%c ", buffer.value[i]);
    }
    printf("\n");

    return 0;
}

// consumeInitial function is used to consume an initial from the buffer
int consumerInitial(char *initial, long int id)
{

	*initial = buffer.value[buffer.next_out];
	buffer.value[buffer.next_out] = '-';
	buffer.next_out = (buffer.next_out + 1) % BUFFER_SIZE;
	printf("\t\t\tconsumer %ld: consumed %c\n", id, *initial);
	
	printf("\t\t\t\t\t\tBuffer: ");
	
    for (int i = 0; i < BUFFER_SIZE; i++)
    {
        printf("%c ", buffer.value[i]);
    }
    printf("\n");
    
    return 0;
	
}

// Producer will iterate PRODUCER_ITERATIONS times and call the insertInitial function to insert an initial to the buffer
// Producer argument param is an integer id of the producer used to distiguish between the multiple producer threads
void *Producer(void *param)
{

    char initials[] = "XQL";
    int length = strlen(initials);

    char initial;
    long int id = (long int)param;

    int j = PRODUCER_ITERATIONS;
    while (j--)
    {

        // wait for random length of time from 0 to 3 seconds
        int randnum = rand() % 4; // range between 0 and 3

        // insert random initial into buffer

        // int item;
        // long int id = (long int)param;

        int randInitial = rand() % length;

        sem_wait(&empty);
        sem_wait(&mutex);
        initial = initials[randInitial];
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

// Consumer will iterate CONSUMER_ITERATIONS times and call the consumeInitial function to insert an initial to the buffer
// Consumer argument param is an integer id of the consumer used to distiguish between the multiple consumer threads
void *Consumer(void *param)
{
    
    char initial;
    long int id = (long int)param; 
    
    int k = CONSUMER_ITERATIONS;
    
    while (k--)
    {
    	sleep(rand() % 6);
    	
    	// read from buffer
    	
    	sem_wait(&full);
    	sem_wait(&mutex);
    	if (consumerInitial(&initial, id))
    		fprintf(stderr, "Error while removing from buffer\n");
    	sem_post(&mutex);
    	sem_post(&empty);
    }

    pthread_exit(0);
}

int main()
{

    buffer.next_in = 0;
    buffer.next_out = 0;

    // Initialize buffer with '-'

    printf("\nInitialize buffer of size %d with '-' \n", BUFFER_SIZE);

    printf("\t\t\t\t\t\tBuffer: ");
    for (int i = 0; i < BUFFER_SIZE; i++)
    {
        buffer.value[i] = '-';
        printf("%c ", buffer.value[i]);
    }
    printf("\n");

    // create producer and consumer threads

    long int i;

    // create consumer threads
    printf("\nCreating %d Consumers and %d Producers\n\n", CONSUMERS, PRODUCERS);

    sem_init(&full, 0, 0);
    sem_init(&empty, 0, BUFFER_SIZE);
    sem_init(&mutex, 0, 1);
    
        // Create the producer threads
    for (i = 0; i < PRODUCERS; i++)
        if (pthread_create(&producerId[i], NULL, Producer, (void *)i) != 0)
        {
            perror("pthread_create");
            abort();
        }

    for (i = 0; i < CONSUMERS; i++)
        if (pthread_create(&consumerId[i], NULL, Consumer, (void *)i) != 0)
        {
            perror("pthread_create");
            abort();
        }
        
        // Wait for threads to complete

    for (i = 0; i < PRODUCERS; i++)
        if (pthread_join(producerId[i], NULL) != 0)
        {
            perror("pthread_join");
            abort();
        }

    for (i = 0; i < CONSUMERS; i++)
        if (pthread_join(consumerId[i], NULL) != 0)
        {
            perror("pthread_join");
            abort();
        }

        
    return 0; 
}
