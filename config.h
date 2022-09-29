#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<getopt.h>
#include<sys/wait.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <time.h>
#include <math.h>
#include <stdbool.h>
#include <signal.h>
#include <errno.h>

/* macros */
#define MAXPROC 20
#define SHM_KEY 0x12345 //shared memory key
#define LEN 100

/* shm variables */
bool *choosing;
int *tickets;
int choose_id, ticket_id, cstest_id;
FILE *cstest = NULL, *logfile = NULL;

/* functions */   
int find_max_val(int *, int);
void deallocate_shm();
void printInfo();

/* signalhandlers */
void signal_timer(int signal);
void signal_abort();

int i, j, c; 
int cpid, status;
