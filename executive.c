#include "ptask.h"
#include <pthread.h>
#include <stdio.h>

void *periodic_task(void *arg);

void *periodic_task(void *arg) {
  int id;

  id = get_task_index(arg);
  wait_for_activation(id);

  printf("%ld: Task %d has started", get_systime(MILLI), id);

  while (1) {
    if (deadline_miss(id)) {
      wait_for_period(id);
    }
  }
}

int main() {

  printf("Startup at time: %ld", get_systime(MILLI));

  task_create(periodic_task, 0, 100, 100, 30, ACT);
  task_create(periodic_task, 1, 200, 200, 20, ACT);
  task_create(periodic_task, 2, 300, 300, 10, ACT);

  for (int i = 0; i < 3; i++) {
    pthread_join(i, NULL);
  }
}
