#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include <sys/shm.h>
#include <sys/sem.h>

#include "semaphore.h"
#include "shm_common.h"

int main(int argc,char*argv[]){
	int shm_id;
	int sem_id;
	void*shared_memory;
	shared_use_st *shared_st;
	if ((shm_id = shmget(SHM_SEED, sizeof(shared_use_st), 0666 | IPC_CREAT)) == -1)  //���������ڴ�
	{    
		fprintf(stderr,"shmget failed\n");		
		exit(EXIT_FAILURE);
	}
	if ((sem_id = semget(ftok(".", 'f'), 1, 0666 | IPC_CREAT)) == -1)//�����ź�������ʼ��
	{     
		fprintf(stderr,"semget failed\n");
		exit(EXIT_FAILURE);
	}
	if((shared_memory=shmat(shm_id,0,0))==(void*)-1)  //�������ڴ����ӵ���ǰ���̵ĵ�ַ�ռ� 
	{     
		fprintf(stderr,"shmat failed\n");              
		exit(EXIT_FAILURE);			
	}
	printf("Memory attached at %X\n", (int)shared_memory);
	shared_st=(shared_use_st*)shared_memory;
	shared_st->end_flag=0; 
	set_semvalue(sem_id,1);
	while(shared_st->end_flag==0)
	{
		semaphore_p(sem_id); //��ȡ�ź���
		printf("intput:");
		scanf("%s",shared_st->shm_sp);
		shared_st->pid=getpid();
		semaphore_v(sem_id);//�ͷ��ź���
		if(strcmp(shared_st->shm_sp,"quit")==0)
		{
			shared_st->end_flag=1;
		}
	}
	sleep(rand() % 2);  //�ȴ�consumer�˳�ѭ���Ժ���ɾ���ź�������ֹPV��������
	del_semvalue(sem_id);     //ɾ���ź���
	if(shmdt(shared_memory)==-1)//�ѹ����ڴ�ӵ�ǰ�����з���
	{
		fprintf(stderr,"shmdt failed\n");
		exit(EXIT_FAILURE);
	}

	exit(EXIT_SUCCESS);
}











