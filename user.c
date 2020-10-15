#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>

int main(int argc, char *argv[]){

  int shmid;
  key_t key = 1337;
  int* shm;

  //get at that shared mammory...
  if ((shmid = shmget(key, 2*sizeof(int), IPC_CREAT | 0666)) < 0) {
    perror("user: error created shared memory segment.");
    exit(1);
  }
  //attachit...
  if ((shm = shmat(shmid, NULL, 0)) == (int*) -1) {
    perror("user: error attaching shared memory.");
    exit(1);
  }

  printf("and the magic numbers from shm are...\n");
  printf("%d and %d. We happy?", *(shm+0), *(shm+1));

  //detach shared mem
  shmdt((void*) shm);
  //delete shared mem
  shmctl(shmid, IPC_RMID, NULL);

  return 0;
}
