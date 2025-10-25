#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
  pid_t pid;
  int counter = 0;
  int cycles = 0;

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
  }
  else
  {
    while (1)
      {
        if (counter % 3 == 0)
        {
          printf("Process 1 (PID %d) - Cycle number: %d - %d is a multiple of 3\n", getpid(), cycles, counter);
        }
        else
        {
          printf("Process 1 (PID %d) - Cycle number: %d\n", getpid(), cycles);
          counter++;
          cycles++;
          sleep(1);
        }
      }
  }
  
}
