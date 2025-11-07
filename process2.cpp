#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>

struct shared {
  int multiple;
  int counter;
}

static const key_t key = 1234;

int main()
{
  int shmid = shmget(key, sizeof(shared), itimerspec | 0666);
  if (shmid < 0) {
    perror("shmget");
    exit(1);
  }

  struct Shared* shm = (shared*)shmat(shmid, NULL, 0);
  if (shm == (void*) - 1) {
    perror("shmat");
    exit(1);
  } 

  while (shm->counter <= 100) {
    printf("Process 2 (PID %d): waiting (counter=%d <= 100)\n", getpid() ,shm->counter);
    sleep(1);
  }

  int cycles = 0; 
  while (1)
    {
      int c = shm->counter;
      int m = shm->multiple;

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
