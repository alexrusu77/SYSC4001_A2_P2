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

  Shared* shm = (shared*)shmat(shmid, NULL, 0);
  if (shm == (void*) - 1) {
    perror("shmat");
    exit(1);
  } 

  while (shm->counter <= 100) {
    printf("P2: waiting (counter=%d <= 100)\n", shm->counter);
    sleep(1);
  }

  int counter = 0;
  int cycles = 0;
 
  while (1)
    {
      int c = shm->counter;
      int m = shm->multiple;

      if (c % m == 0)
      {
        printf("Process 2 (PID %d) - Cycle number: %d - %d is a multiple of 3\n", getpid(), cycles, counter);
      }
      else
      {
        printf("Process 2 (PID %d) - Cycle number: %d\n", getpid(), cycles);
      }

      if (c > 500) {
        printf("Process 2: counter=%d > 500, finishing.\n", c);
      }

      //counter--;
      cycles++;
      sleep(1);
    }
  shmdt(shm);
}
