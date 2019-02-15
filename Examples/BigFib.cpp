#include "../BigNum.h"
#include <time.h>
#include <math.h>
#include <pthread.h>
#include <semaphore.h>

BigNum getFib(int num);
void *change_k1_val(void *unused);
void *change_k_val(void *unused);

pthread_t threads[2];
sem_t sems[2];
sem_t sem_main;

BigNum k_val = 1;
BigNum k1_val = 1;
BigNum k_val_n = 1;
BigNum k1_val_n = 1;

int main(int argc, char *argv[])
{
	if (argc > 1)
	{
		clock_t start = clock();
		int i = atoi(argv[1]);
		sem_init(&sem_main, 0, 0);
		sem_init(&sems[0], 0, 0);
		sem_init(&sems[1], 0, 0);
		pthread_create(&threads[0], NULL, change_k_val, NULL);
		pthread_create(&threads[1], NULL, change_k1_val, NULL);
		BigNum fib = getFib(i);
		fib.print();

		clock_t end = clock();
		std::cout << "Total Length of Output: " << fib.length() << std::endl;
		std::cout << "Time to complete: " << (double)(end - start) / CLOCKS_PER_SEC << " s" << std::endl;
	}
	else
	{
		std::cout << "Enter the Fibonacci to generate as an argument. Aborting..." << std::endl;
	}
	return 0;
}

BigNum getFib(int num)
{
	char *steps = (char *)malloc((int)ceil(log2(num) + 1) * sizeof(char));
	int cur = num;
	int count;
	for (count = 0; cur > 1; count++)
	{
		steps[count] = cur % 2;
		cur = cur / 2;
	}
	
	BigNum temp(0);

	int i;
	for (i = count - 1; i >= 0; i--)
	{
		// k_val_n = k_val * ((2 * k1_val) - k_val);
		// k1_val_n = (k1_val * k1_val) + (k_val * k_val);
		// k_val = k_val_n;
		// k1_val = k1_val_n;
		sem_post(&sems[0]);
		sem_post(&sems[1]);
		sem_wait(&sem_main);
		sem_wait(&sem_main);
		sem_post(&sems[0]);
		sem_post(&sems[1]);
		sem_wait(&sem_main);
		sem_wait(&sem_main);
		if (steps[i])
		{
			temp = k_val + k1_val;
			k_val = k1_val;
			k1_val = temp;
		}
	}
	free(steps);
	return k_val;
}

void *change_k_val(void *unused){
	while(true){
		sem_wait(&sems[0]);
		k_val_n = k_val * ((2 * k1_val) - k_val);
		sem_post(&sem_main);
		sem_wait(&sems[0]);
		k_val = k_val_n;
		sem_post(&sem_main);
	}
	return NULL;
}

void *change_k1_val(void *unused){
	while(true){
		sem_wait(&sems[1]);
		k1_val_n = (k1_val * k1_val) + (k_val * k_val);
		sem_post(&sem_main);
		sem_wait(&sems[1]);
		k1_val = k1_val_n;
		sem_post(&sem_main);
	}
	return NULL;
}

