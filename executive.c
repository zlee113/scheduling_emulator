#include "ptask.h"
#include <pthread.h>
#include <stdio.h>
#include <time.h>

extern struct task_par tp[MAX_TASKS];
extern int current_task;

void *periodic_task(void *arg);
void *periodic_task_nonpreemptive(void *arg);

void *periodic_task(void *arg) {
  int id;

  id = get_task_index(arg);

  while (1) {
    /* start only after the period ends */
    wait_for_period(id);
    tp[id].finished = false;

    /* Check current task should run */
    if (current_task == -1) {
      task_set_next_task();
      continue;
    } else if (id != current_task) {
      wait_for_activation(id);
    } 
    /* Check Deadline */
    if (deadline_miss(id)) {
      printf("%ld: Task %d missed it's deadline\n", get_systime(MILLI), id);
    }

    /* Start actual task */
    long start = get_systime(MILLI);
    printf("%ld: Task %d has started\n", start, id);

    while ((get_systime(MILLI) - start) < tp[id].wcet) {
      /* Do nothing */
    }

    /* Get end */
    long end = get_systime(MILLI);
    printf("%ld: Task %d finished with duration: %ld ms \n", get_systime(MILLI),
           id, end - start);

    tp[id].finished = true;
    task_set_next_task();
  }
  return NULL;
}

int main() {
  printf("Initializing ptask func\n");
  ptask_init(0);

  printf("Startup at time: %ld\n", get_systime(MILLI));

  current_task = 0;
  task_create(periodic_task, 0, 100, 100, 30, 50, NON_ACT);
  task_create(periodic_task, 1, 300, 300, 20, 100, NON_ACT);
  task_create(periodic_task, 2, 1000, 1000, 10, 300, NON_ACT);

  for (int i = 0; i < 3; i++) {
    if (tp[i].tid) {
      pthread_join(tp[i].tid, NULL);
    } else {
      printf("Thread %d has invalid TID\n", i);
    }
  }
}
