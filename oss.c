/*
Alberto Maiocco
CS4760 Project 3
10/20/2020
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include <sys/types.h>


FILE* outfile;

//this struct might not be a great idea...
typedef struct shareClock{
  int secs;
  int nano;
}shareClock;

int main(int argc, char *argv[]) {

  int maxProc = 5;
  char* filename = "log.txt";
  int maxSecs = 20;

  int optionIndex;
  while ((optionIndex = getopt(argc, argv, "hc:l:t:")) != -1) {
    switch (optionIndex) {
      case 'h':
          printf("Welcome to the Valid Argument Usage Dimension\n");
          printf("- = - = - = - = - = - = - = - = - = - = - = - = -\n");
          printf("-h            : Display correct command line argument Usage\n");
          printf("-c <int>      : Indicate the maximum total of child processes spawned. (Default 5)\n");
          printf("-l <filename> : Indicate the number of children allowed to exist in the system at the same time. (Default 2)\n");
          printf("-t <int>      : The time in seconds after which the process will terminate, even if it has not finished. (Default 20)\n");
          printf("Usage: ./oss [-h | -c x -l filename -t z]\n");
          exit(0);
        break;

      case 'c':
        maxProc = atoi(optarg);
        if (maxProc <= 0) {
          perror("master: maxProc <= 0. Aborting.");
          exit(1);
        }
        if (maxProc > 20) maxProc = 20;
        break;

      case 'l':
        if (optarg == NULL) {
          optarg = "log.txt";
        }
        filename = optarg;
        break;

      case 't':
        maxSecs = atoi(optarg);
        if (maxSecs <= 0) {
          perror("master: maxSecs <= 0. Aborting.");
          exit(1);
        }
        break;

      case '?':
        if(isprint(optopt)) {
          fprintf(stderr, "Uknown option `-%c`.\n", optopt);
          perror("Error: Unknown option entered.");
          return 1;
        }
        else {
          fprintf (stderr, "Unkown option character `\\x%x`.\n", optopt);
          perror("Error: Unknown option character read.");
          return 1;
        }
        return 1;

      default:
        abort();

    }
  }
  printf("getopt test: -c: %d -l: %s -t: %d\n", maxProc, filename, maxSecs);

  //open log file for editing
  outfile = fopen(filename, "a+");
  if (!outfile) {
    perror("oss: error opening output log.");
    exit(1);
  }

  //next step: set up shared memory!
  //We'll do share clock with test values 612 and 6633
  int shmid;
  key_t key = 1337;
  int* shm; // <- this should be the shareClock, right? or its the shared int?
  //create shared mem segment...
  //sizeof int for seconds and nanoseconds... dont need a struct for clock?
  if ((shmid = shmget(key, 2*sizeof(int), IPC_CREAT | 0666)) < 0) {
    perror("oss: error created shared memory segment.");
    exit(1);
  }
  //attach segment to dataspace...
  if ((shm = shmat(shmid, NULL, 0)) == (int*) -1) {
    perror("oss: error attaching shared memory.");
    exit(1);
  }

  //here we'll have to write to shared memory...
  *(shm+0) = 0;
  *(shm+1) = 0;

  //NEXT TIME ON DRAGON BALL Z- GOKU SETS UP USER.C TO READ SHARED MEMORY.
  //exec y00zer...

  //Using the interupt handlers...
  // alarm for max time and ctrl-c
  signal(SIGALRM, interruptHandler);
  signal(SIGINT, interruptHandler);
  alarm(maxSecs);

  //so we want one child to deal with this,
  //then we set up message queue
  //this part evolves into Critsec at lvl 35. Gotta start grinding!
  pid_t childpid = 0;
  int status = 0;
  int pid = 0;
  if((childpid = fork()) < 0) {
    perror("./oss: ...it was a stillbirth.");
    exit(-1);
  } else if (childpid == 0) {
    printf("Red %d standing by!\n", getpid());
    char *args[]={"./user", "25", "612"};
    execvp(args[0], args);
  }

  //don't want to destroy shm too fast, so we wait for child to finish.
  do {
    pid = waitpid(-1, &status, WNOHANG);
  } while(pid == 0);

  //de-tach and de-stroy shm..
  printf("And we're back! Miss me?\n");
  //detach shared mem
  shmdt((void*) shm);
  //delete shared mem
  shmctl(shmid, IPC_RMID, NULL);
  printf("shm has left us for Sto'Vo'Kor\n");

  printf("fin.\n");
  return 0;
}

static void interruptHandler() {
  key_t key = 1337;
  int* shm;
  int shmid;
  if ((shmid = shmget(key, 2*sizeof(int), IPC_CREAT | 0666)) < 0) {
    perror("oss: error created shared memory segment.");
    exit(1);
  }
  if ((shm = shmat(shmid, NULL, 0)) == (int*) -1) {
    perror("oss: error attaching shared memory.");
    exit(1);
  }
  //close file...
  fprintf(outfile, "Interrupt in yo face @ %ds, %dns\n", *(shm+0), *(shm+1));
  fclose(outfile);
  //cleanup shm...
  shmctl(shmid, IPC_RMID, NULL);
  //eliminate any witnesses...
  kill(0, SIGKILL);
  exit(0);
}
