#include "../BigNum.h"
#include <time.h>
#include <pthread.h>
#include <semaphore.h>

#define NUM_THREADS 4

int val = 1;
int maxval = 10;
int done = 0;

pthread_t threads[NUM_THREADS];
sem_t child_sem;
sem_t parent_sem;
BigNum nums[NUM_THREADS];

void *mult(void *thread_num_v){
	int thread_num = *(int *)thread_num_v;
	sem_post(&parent_sem);
	int internal_val;
	nums[thread_num] = 1;
	while(!done){
		sem_wait(&child_sem);
		internal_val = val;
		sem_post(&parent_sem);
		if(internal_val < maxval){
			nums[thread_num] = nums[thread_num] * internal_val;
		}
	}

return NULL;
}




int main(int argc, char *argv[]) {
	if (argc > 1) {
		time_t start = time(NULL);
		int num = atoi(argv[1]);
		int i;
		sem_init(&parent_sem, 0, 0);
		sem_init(&child_sem, 0, 0);
		for(i = 0; i < NUM_THREADS; ++i){
			pthread_create(&threads[i], NULL, mult, (void *)&i);
			sem_wait(&parent_sem);
		}

		BigNum total(1);
		maxval = num + 1;
		for (val = 1; val < maxval; ++val) {
			sem_post(&child_sem);
			sem_wait(&parent_sem);
		}
		done = 1;
		for(i = 0; i < NUM_THREADS; ++i){
			sem_post(&child_sem);
		}
		for(i = 0; i < NUM_THREADS; ++i){
			pthread_join(threads[i], NULL);
			total = total * nums[i];
		}
		std::cout << total << std::endl;
		time_t end = time(NULL);
		std::cout << "Total Length of Output: " << total.length() << std::endl;
		std::cout << "Time to complete: " << (end - start) << "s" << std::endl;
	}
	else {
		std::cout << "Enter numbet to find the factorial of as an argument. Aborting..." << std::endl;
	}
	return 0;
}
