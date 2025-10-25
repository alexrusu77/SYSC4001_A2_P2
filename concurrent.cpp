#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
  pid_t pid = fork();

  if (pid < 0)
  {
    perror("fork failed");
    exit(1);
  }

  if (pid == 0)
  {
    // Child process - Process 2
    int counter_child = 0;
    while(1)
      {
        printf("Process 2 (PID %d): %d\n", getpid(), counter_child++);
        sleep(1);
      }
  }
  else
  {
    int counter_parent = 0;
    while(1)
      {
        printf("Process 1 (PID %d): %d\n", getpid(), counter_parent++);
        sleep(1);
      }
  }
}
