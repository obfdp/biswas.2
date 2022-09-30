#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>


void oss();
void worker();

#define SHMKEY  859047     /* oss and worker agree on common key.*/
#define BUFF_SZ sizeof ( int )

int opt, flag = 0, max_proc = 18, def_time = 100, errno, pid = 0, * oss = NULL, * worker = NULL;
time_t start = 0;


int main(int argc, char * argv[]) {
  system("clear");

  signal(SIGINT, signal_abort); // Abort for Ctrl+C 
  signal(SIGALRM, signal_timer); //Abort for end of termination time 

  /* Parsing Command Line argument */
  while ((opt = getopt(argc, argv, "hn:")) != -1) {
    switch (opt) {
    case 'h':
      printInfo();
      break;
    case 'n':
      max_proc = atoi(argv[2]);
      if (max_proc > MAXPROC) {
        perror("Numbers of Processes can not exceed 18. Number of processes range: [1,18]");
        max_proc = 18;
      }
      break;
    case 's':
      max_child = atoi(argv[3]);
      if(max_child> 5 )
        perror("Numbers of child can not exceed 5. Number of child range: [1,5]");
      break;
    case 'm':
      flag = 1;
      def_time = atoi(optarg);
      if (def_time < 1) {
        perror("Termination time is not valid. Time Range to choose:[1,10000000]");
      }
      break;
    case '?':
      if (opt == 'c')
        perror("Error: Option requires an argument");
      else
        perror("Error: Unknown option character");
      return 1;

    }
}
  if (flag == 0) {
    max_proc = atoi(argv[1]);
    if (max_proc > MAXPROC) {
      perror("Warning: Numbers of Processes can not exceed 18");
      max_proc = 18;
    }
  }

  fprintf(stderr, "Timer value selected: %d\n", def_time);
  fprintf(stderr, "Number of processes: %d\n", max_proc);

  oss = malloc(sizeof(int) * max_proc);
  worker = malloc(sizeof(int) * max_proc);

  /* Creating a shared memory segment */
  const int sh_key = ftok("main.c",0);
  int shm_id = shmget( sh_key , sizeof(int) * 10 , IPC_CREAT | 0666  );
  if (shm_id <= 0) {
   fprintf(stderr,"Shared memory get failed\n");
   exit(1);
  }
  int *shm_ptr = shmat( shm_id , 0 , 0 );
  if (shm_ptr <= 0) {
   fprintf(stderr,"Shared memory attach failed\n");
   exit(1);
  }

/* Deallocate Shared Memory */
  shmdt( shm_ptr );    // Detach from the shared memory segment
  shmctl( shm_id, IPC_RMID, NULL ); // Free shared memory segment shm_id
  exit(EXIT_SUCCESS);
}
