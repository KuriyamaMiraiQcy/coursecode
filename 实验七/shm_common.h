#ifndef __SHM_COMMON_H__
#define __SHM_COMMON_H__

#define SHM_SEED 1000
#define MAX_SHM_SIZE 2048

typedef struct shared_use_st
{
  int end_flag;            
  char shm_sp[MAX_SHM_SIZE];
  int pid; 		    
}shared_use_st;

#endif
