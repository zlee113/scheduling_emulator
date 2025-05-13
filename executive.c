#include "ptask.h"
#include <pthread.h>
#include <stdio.h>

extern struct task_par tp[MAX_TASKS];

void *periodic_task(void *arg);

void *periodic_task(void *arg) {
  int id;

  id = get_task_index(arg);
  wait_for_activation(id);


  while (1) {
    printf("%ld: Task %d has started", get_systime(MILLI), id);
    if (deadline_miss(id)) {
      printf("%ld: Task %d missed it's deadline", get_systime(MILLI), id);
    }

    

    /* wait for another period */
    wait_for_period(id);
  }
}

int main() {
  printf("Intializing ptask func\n");
  ptask_init(0);

  printf("Startup at time: %ld\n", get_systime(MILLI));

  task_create(periodic_task, 0, 100, 100, 30, ACT);
  task_create(periodic_task, 1, 300, 300, 20, ACT);
  task_create(periodic_task, 2, 1000, 1000, 10, ACT);

  for (int i = 0; i < 3; i++) {
    pthread_join(tp[i].tid, NULL);
  }
}
