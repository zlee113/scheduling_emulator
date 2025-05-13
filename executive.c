#include "ptask.h"
#include <pthread.h>
#include <stdio.h>
#include <time.h>

extern struct task_par tp[MAX_TASKS];

void *periodic_task(void *arg);
void *periodic_task_nonpreemptive(void *arg);

void *periodic_task(void *arg) {
  int id;

  id = get_task_index(arg);
  wait_for_activation(id);


  while (1) {
    long start = get_systime(MILLI);
    printf("%ld: Task %d has started\n", start, id);
    if (deadline_miss(id)) {
      printf("%ld: Task %d missed it's deadline\n", get_systime(MILLI), id);
    }

    while ((get_systime(MILLI) - start) < tp[id].wcet){
      /* Do nothing */
    }


    long end = get_systime(MILLI);
    printf("%ld: Task %d finished with duration: %ld ms \n", get_systime(MILLI), id, end-start);
    /* wait for another period */
    wait_for_period(id);
  }
  return NULL;
}

void *periodic_task_nonpreemptive(void *arg){

}

int main() {
  printf("Initializing ptask func\n");
  ptask_init(0);

  printf("Startup at time: %ld\n", get_systime(MILLI));

  task_create(periodic_task, 0, 100, 50, 30, 50, ACT);
  usleep(1000);
  task_create(periodic_task, 1, 300, 300, 20, 100,ACT);
  usleep(1000);
  task_create(periodic_task, 2, 1000, 1000, 10, 300, ACT);

  for (int i = 0; i < 3; i++) {
    if (tp[i].tid) {
    pthread_join(tp[i].tid, NULL);
    } else {
      printf("Thread %d has invalid TID\n", i);
    }
  }
}
