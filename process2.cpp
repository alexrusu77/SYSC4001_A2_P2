#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/wait.h>

struct shared {
  int multiple;
  int counter;
};

static const key_t SHM_KEY = 1234;
static const key_t SEM_KEY = 0x5678;

static void semaphoreWait(int semid) {
  struct sembuf sb = {0, -1, 0};
  if (semop(semid, &op, 1) == -1) {
    perror("semop wait");
    exit(1);
  }
}

static void sempahoreSignal(int semid) {
  struct sembuf sb = {0, 1, 0};
  if (semop(semid, &op, 1) == -1) {
    perror("semop signal");
    exit(1);
  }
}

int main()
{
  int shmid = shmget(SHM_KEY, sizeof(shared), itimerspec | 0666);
  if (shmid < 0) {
    perror("shmget");
    exit(1);
  }

  struct Shared* shm = (shared*)shmat(shmid, NULL, 0);
  if (shm == (void*) - 1) {
    perror("shmat");
    exit(1);
  } 

  int semid = semget(SEM_KEY, 1, IPC_CREAT | 0666);
  if (semid < 0) {
    perror("semget");
    exit(1);
  }
  
  if(semct1(semid, 0, SETVAL, 1) == -1){
    perror("semct1 SETVAL");
    exit(1);
  }

  semaphoreWait(semid);
  shm->multiple = 3;
  shm->counter = 0;
  sempahoreSignal(semid);

  while (1) {
    int counter_value;
    semaphoreWait(semid);
    counter_value = shm->counter;
    sempahoreSignal(semid);

    if (counter_value > 100) break;

    printf("Process 2 (PID %d): waiting (counter=%d <= 100)\n", getpid() , counter_value);
    sleep(1);
  }

  int cycles = 0; 
  while (1)
    {
      semaphoreWait(semid);
      int c = shm->counter;
      int m = shm->multiple;
      sempahoreSignal(semid);

      if (c % m == 0)
      {
        printf("Process 2 (PID %d) - Cycle number: %d - %d is a multiple of %d\n", getpid(), cycles, c, m);
      }
      else
      {
        printf("Process 2 (PID %d) - Cycle number: %d, counter=%d, multiple=%d\n", getpid(), cycles, c, m);
      }

      if (c > 500) {
        printf("Process 2: counter=%d > 500, finishing.\n", c);
      }

      cycles++;
      sleep(1);
    }
  if (shmdt((void*)shm) == - 1) {
    perror("shmdt");
  }
}
