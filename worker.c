#include "config.h"

int main(int argc, char *argv[])
{
    /* signal handling */
    signal(SIGTERM, deallocate_shm);
 
    /* Parsing Command Line argument */  
    int p_id = atoi(argv[1]); 
    int num_proc = atoi(argv[2]); 

    /* Ensuring that IPC_CREAT created the segment in oss */
    choose_id = shmget(ftok("Makefile", '1'), sizeof(bool) * num_proc, IPC_EXCL);
    ticket_id = shmget(ftok("Makefile", '2'), sizeof(int) * num_proc, IPC_EXCL);    
    cstest_id = shmget(ftok("Makefile", '3'), sizeof(FILE) * num_proc, IPC_EXCL);

    if (choose_id == -1 || ticket_id == -1 || cstest_id == -1){
        perror("worker: Error: Shared memory allocation failed");
    }
    
    choosing = (bool *)shmat(choose_id, NULL, 0);
    tickets = (int *)shmat(ticket_id, NULL, 0);
    cstest = (FILE *)shmat(cstest_id, NULL, 0);
    
   if (choosing == (void *) -1 || tickets == (void *) -1 || cstest == (void *) -1 ) {
      perror("worker: Error: Shared memory attachment failed");
   }

   /* Deallocate Shared Memory */
void deallocate_shm()
{
    if (cstest != NULL) fclose(cstest);
    if (logfile != NULL) fclose(logfile);

    if (shmdt(choosing) == -1 || shmdt(tickets) == -1) {
        perror("oss: Error: shmdt failed to detach memory");
     }

   exit(EXIT_SUCCESS);
}

}
