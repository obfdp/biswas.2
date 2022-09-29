nclude "config.h"

int opt, flag = 0, max_proc = 18, def_time = 100, errno, pid = 0, * oss = NULL, * worker = NULL;
time_t start = 0;

int main(int argc, char * argv[]) {

	system("clear");
  
  signal(SIGINT, signal_abort); // Abort for Ctrl+C 
  signal(SIGALRM, signal_timer); //Abort for end of termination time 

  // Parsing Command Line argument 
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

  // Creating a shared memory segment 
  choose_id = shmget(ftok("Makefile", '1'), sizeof(bool) * max_proc, IPC_CREAT | 0666);
  ticket_id = shmget(ftok("Makefile", '2'), sizeof(int) * max_proc, IPC_CREAT | 0666);
  cstest_id = shmget(ftok("Makefile", '3'), sizeof(FILE) * max_proc, IPC_CREAT | 0666);

  printf("In case of execution problem, Use the below command to free shared memory by using ipcrm \n");
  printf("ipcrm -m %d\n", choose_id);
  printf("ipcrm -m %d\n", ticket_id);

  if (choose_id == -1 || ticket_id == -1 || cstest_id == -1) {
    perror("Error: Shared memory allocation failed");
  }

  choosing = (bool * ) shmat(choose_id, NULL, 0);
  tickets = (int * ) shmat(ticket_id, NULL, 0);
  cstest = (FILE * ) shmat(ticket_id, NULL, 0);

  if (choosing == (void * ) - 1 || tickets == (void * ) - 1 || cstest == (void * ) - 1) {
    perror("Error: Shared memory attachment failed");
  }

  for (i = 0; i < max_proc; i++) {
    choosing[i] = false;
    tickets[i] = 0;
  }

  // Deallocate Shared Memory 
  void deallocate_shm(){
  if (shmdt(choosing) == -1 || shmdt(tickets) == -1) {
      perror("Error: shmdt failed to detach memory");
  }
  if (shmctl(choose_id, IPC_RMID, 0) == -1 || shmctl(ticket_id, IPC_RMID, 0) == -1 ) {
      perror("Error: shmctl failed to delete shared memory");
  }  
}
