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

#define SHMSZ 256

FILE* outfile;

int main(int argc, char *argv[]) {

  int maxProc = 5;
  char* filename;
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

  return 0;
}
