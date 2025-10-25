#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
  int counter = 0;
  int cycles = 0;
 
  while (1)
    {
      if (counter % 3 == 0)
      {
        printf("Process 2 (PID %d) - Cycle number: %d - %d is a multiple of 3\n", getpid(), cycles, counter);
      }
      else
      {
        printf("Process 2 (PID %d) - Cycle number: %d\n", getpid(), cycles);
      }
      counter--;
      cycles++;
      sleep(1);
    }
  
}
