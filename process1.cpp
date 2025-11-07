#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/ipc.h>


struct shared {
  int multiple;
  int counter;
};

static const key_t key = 1234

int main()
{
  int shmid = shmget(key, sizeof(shared), IPC_CREAT | 0666);
  if (shmid < 0) {
    perror("shmget");
    exit(1);
  }

  struct Shared* shm = (shared*)shmat(shmid, NULL, 0);
  if (shm == (void*) - 1) {
    perror("shmat");
    exit(1);
  } 

  shm->multiple = 3;
  shm->counter = 0;

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
        int m = shm->multiple;
        int c = ++(shm->counter);

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
  if (shmct1(shmid,IPC_RMID,NULL) == -1) {
    perror("shmct1 IPC_RMID");
  }
  return 0;
}
