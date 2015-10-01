#include <iostream>

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <time.h>

using namespace std;

struct shared
{
	int number;
	pthread_mutex_t mutex;
	pthread_cond_t cond;
	bool written;

};

void *write(void *);
void *read(void *);

int
main(int argc, char **argv)
{
	//create threads
	pthread_t A, B;

	//create shared obejct
	struct shared share;

	//initialize mutex
	pthread_mutex_init(&share.mutex, NULL);
	pthread_cond_init(&share.cond, NULL);
	share.written = false;
	share.number = 0;
	srand(time(NULL));

	//create the threads
	pthread_create(&A, NULL, &write, &share);
	pthread_create(&B, NULL, &read, &share);
	

	//wait for both threads to terminate
	pthread_join(A, NULL);
	pthread_join(B, NULL);
	

	exit(0);
}

void *
write(void *vptr)
{
	struct shared* s = (struct shared*) vptr;



	
	pthread_mutex_lock(&s->mutex);

	int random = rand() % 100 + 1;
	s->number = random;
	s->written = true;
	cout << "Storing: " << random  << endl;

	pthread_cond_signal(&s->cond);
	pthread_mutex_unlock(&s->mutex);



	usleep(1);
}

void *
read(void *vptr)
{
	struct shared* s = (struct shared*) vptr;




	pthread_mutex_lock(&s->mutex);

	while(s->written == false)
	{
		pthread_cond_wait(&s->cond, &s->mutex);
	}
	
	cout << "Reading: " << s->number  << endl;

	pthread_mutex_unlock(&s->mutex);
	usleep(1);

}