#include "semaphore.h"

void init(){
	producerCount = 0;
	consumerCount = 0;

	if (sem_init(&consume_ok, 0, 0) != 0){
		printf("%s[ERROR]Cannot init semaphore : consume_ok\n", T_RED);
		exit(0);
	}

	if (sem_init(&produce_ok, 0, PRODUCE_ONCE) != 0){
		printf("%s[ERROR]Cannot init semaphore : produce_ok\n", T_RED);
		exit(0);
	}

	if (sem_init(&buffer_lock, 0, 1) != 0){
		printf("%s[ERROR]Cannot init semaphore : buffer_lock\n", T_RED);
		exit(0);
	}
}

void *producer(void *args){
	int i;
	int producerID = ++producerCount;
	int item;

	for (i = 0; i < PRODUCE_TIME; i++){
		item = (random() % 100) + producerID;
		usleep(item);

		sem_wait(&produce_ok);
		sem_wait(&buffer_lock);

		if (insertItem(item) == 0){
			printf("%s[PRODUCER_ERROR] insertItem Failed%s\n", T_RED, T_DEFAULT);
		}
		else{
			printf("%sproducer %d add CQ %d%s\n", T_YELLOW, producerID, item, T_DEFAULT);
		}

		sem_post(&buffer_lock);
		sem_post(&consume_ok);
	}
}

void *consumer(void *args){
	int i;
	int consumerID = ++consumerCount;
	int time;

	for (i = 0; i < CONSUME_TIME; i++){
		int item;
		time = (random() % 100) + consumerID;
		usleep(time);

		sem_wait(&consume_ok);
		sem_wait(&buffer_lock);

		if ((item = takeItem()) == -1){
			printf("%s[CONSUMER_ERROR] takeItem Failed%s\n", T_RED, T_DEFAULT);
		}
		else{
			printf("%sconsumer %d take item from CQ %d%s\n", T_GREEN, consumerID, item, T_DEFAULT);
		}

		sem_post(&buffer_lock);
		sem_post(&produce_ok);
	}
}


