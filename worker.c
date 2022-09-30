#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

/* incrementing share clock by 10000000*/
int main(int argc, char *argv[])
{
int i;
for (i =0; i < argc; i++){
  paddr=paddr*10000000
  printf("worker_pid %d",getpid());
  printf(": Iteration %i",i);
  printf(": Incrementing by %p",paddr);
  printf("|%s| ", argv[i]);
}

printf("\nChild is now ending.\n");
sleep(3);
return EXIT_SUCCESS;

}


