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
  if (semop(semid, &sb, 1) == -1) {
    perror("semop wait");
    exit(1);
  }
}

static void sempahoreSignal(int semid) {
  struct sembuf sb = {0, 1, 0};
  if (semop(semid, &sb, 1) == -1) {
    perror("semop signal");
    exit(1);
  }
}

int main()
{
  int shmid = shmget(SHM_KEY, sizeof(shared), IPC_CREAT | 0666);
  if (shmid < 0) {
    perror("shmget");
    exit(1);
  }

  struct shared* shm = (struct shared*)shmat(shmid, NULL, 0);
  if (shm == (void*) - 1) {
    perror("shmat");
    exit(1);
  } 

  int semid = semget(SEM_KEY, 1, IPC_CREAT | 0666);
  if (semid < 0) {
    perror("semget");
    exit(1);
  }
  
  if(semctl(semid, 0, SETVAL, 1) == -1){
    perror("semctl SETVAL");
    exit(1);
  }

  semaphoreWait(semid);
  shm->multiple = 3;
  shm->counter = 0;
  sempahoreSignal(semid);

  pid_t pid; //Create a new process
  pid = fork();

  if (pid < 0)
  {
    perror("fork failed");
    exit(1);
  }

  if (pid == 0)
  {
    execl("./process2", "process2", NULL);
    perror("execl failed");
    exit(1);
  }
  else
  {
    int cycles = 0;
    while (1)
      {
        semaphoreWait(semid);
        int m = shm->multiple;
        int c = ++(shm->counter);
        sempahoreSignal(semid);

        printf("Process 1 (PID %d): child PID %d started; waiting...\n", getpid(), pid);

        if (c % m == 0)
        {
          printf("Process 1 (PID %d) - Cycle number: %d - %d is a multiple of %d\n", getpid(), cycles, c, m);
        }
        else
        {
          printf("Process 1 (PID %d) - Cycle number: %d, counter: %d (multiple=%d)\n", getpid(), cycles, c, m);
        }

        if (c > 500) {
          printf("Process 1: counter = %d > 500, finishing...\n", c);
        }

        cycles++;
        sleep(1);
      }
  }

  int status = 0;
  if (waitpid(pid, &status, 0) == -1) {
    perror("waitpid");
    exit(1);
  }

  if (shmdt((void*)shm) == - 1) {
    perror("shmdt");
  }
  if (shmctl(shmid,IPC_RMID,NULL) == -1) {
    perror("shmctl IPC_RMID");
  }
  if (semctl(semid, 0, IPC_RMID) == -1) {
    perror("semctl IPC_RMID");
  }

  return 0;
}
