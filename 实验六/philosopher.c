#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

pthread_mutex_t changeState;
pthread_cond_t eater[5];
int state[5];//0表示思考，1表示吃

void eat(int i)
{
	int l = (i + 5 - 1) % 5;//左边的哲学家
	int r = (i + 1) % 5;//右边的哲学家
	pthread_mutex_lock (&changeState);
	while (state[l] == 1 || state[r] == 1) //等待旁边人吃完
	{
		pthread_cond_wait (&eater[i], &changeState);
    }
    state[i] = 1; //改变状态
   	printf("No. %d stop thinking \tstart eating\n", i);
    pthread_mutex_unlock (&changeState);
    usleep ((rand() % 100 + 50) * 20000);
}

void think(int i)
{
	int l = (i + 5 - 1) % 5;
	int r = (i + 1) % 5;
	pthread_mutex_lock (&changeState);
	state[i] = 0; //改变状态
	printf("No. %d stop eating \tstart thinking\n", i);
	pthread_cond_signal (&eater[l]);//通知左边的哲学家线程
	pthread_cond_signal (&eater[r]);//通知右边的哲学家线程
	pthread_mutex_unlock (&changeState);
	usleep ((rand() % 100 + 50) * 20000);
}

void* phi(void* arg)
{
	int i = (int) arg;
	for (;;) {
		eat(i);
		think(i);
	}
	return NULL;
}

int main(void)
{
	pthread_t p0, p1, p2, p3, p4;
	pthread_mutex_init(&changeState, NULL);
	int i;
	for (i = 0; i < 5; i++) {
		pthread_cond_init(&eater[i], NULL);
	}
	pthread_create(&p0, NULL, phi, (void*)0);
	pthread_create(&p1, NULL, phi, (void*)1);
	pthread_create(&p2, NULL, phi, (void*)2);
	pthread_create(&p3, NULL, phi, (void*)3);
	pthread_create(&p4, NULL, phi, (void*)4);

	pthread_join(p0, NULL);
	pthread_join(p1, NULL);
	pthread_join(p2, NULL);
	pthread_join(p3, NULL);
	pthread_join(p4, NULL);

	return 0;
}