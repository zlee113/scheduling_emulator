#include "ptask.h"

/* Global variation for array holding every task */
struct task_par tp[MAX_TASKS] = {0};

/* Global timing variable */
struct timespec ptask_t0 = {0};

/* Global current task variable */
int current_task = 0;

/* Global policy variable */
int ptask_policy;

void ptask_init(int policy) {
  ptask_policy = policy;
  clock_gettime(CLOCK_MONOTONIC, &ptask_t0);

  for (int i = 0; i < MAX_TASKS; i++)
    sem_init(&tp[i].asem, 0, 0);
}

int task_create(void *(*task)(void *), int index, int period, int deadline,
                int priority, long wcet, int act_flag) {
  pthread_attr_t myatt;
  struct sched_param mypar;
  int tret;

  printf("Creating task at index: %d\n", index);

  if (index >= MAX_TASKS)
    return -1;

  tp[index].arg = index;
  tp[index].period = period;
  tp[index].deadline = deadline;
  tp[index].priority = priority;
  tp[index].wcet = wcet;
  tp[index].dmiss = 0;
  tp[index].finished = false;
  tp[index].alive = true;

  pthread_attr_init(&myatt);
  pthread_attr_setinheritsched(&myatt, PTHREAD_EXPLICIT_SCHED);
  pthread_attr_setschedpolicy(&myatt, SCHED_FIFO);
  mypar.sched_priority = tp[index].priority;
  pthread_attr_setschedparam(&myatt, &mypar);

  tret = pthread_create(&tp[index].tid, &myatt, task, (void *)(&tp[index]));
  printf("Task %d created with TID %ld\n", index, (long)tp[index].tid);

  if (tret != 0) {
    printf("Error creating thread %d, error code: %d, %s\n", index, tret,
           strerror(tret));
    return tret; // Return error code
  }

  if (act_flag == ACT) {
    task_activate(index);
  }

  return tret;
}

int get_task_index(void *arg) {
  struct task_par *tpar;

  tpar = (struct task_par *)arg;
  return tpar->arg;
}

void wait_for_activation(int id) {
  struct timespec t;

  printf("Waiting for Task %d\n", id);
  sem_wait(&tp[id].asem);

  clock_gettime(CLOCK_MONOTONIC, &t);
  time_copy(&(tp[id].at), t);
  time_copy(&(tp[id].dl), t);
  time_add_ms(&(tp[id].at), tp[id].period);
  time_add_ms(&(tp[id].dl), tp[id].deadline);
}

void task_activate(int id) {
  printf("Task %d activated\n", id);
  sem_post(&tp[id].asem);
}

int deadline_miss(int id) {
  struct timespec now;
  clock_gettime(CLOCK_MONOTONIC, &now);

  if (time_cmp(now, tp[id].dl) > 0) {
    tp[id].dmiss++;
    return 1;
  }
  return 0;
}

void wait_for_period(int id) {
  clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &(tp[id].at), NULL);

  time_add_ms(&(tp[id].at), tp[id].period);
  time_add_ms(&(tp[id].dl), tp[id].period);
}

void wait_for_task_end(int id);

int task_get_period(int id) { return tp[id].period; }

int task_get_deadline(int id) { return tp[id].deadline; }

void task_get_attime(int id, struct timespec *at) { time_copy(at, tp[id].at); }

void task_get_adline(int id, struct timespec *dl) { time_copy(dl, tp[id].dl); }

void task_set_period(int id, int period) { tp[id].period = period; }

void task_set_deadline(int id, int deadline) { tp[id].deadline = deadline; }

void task_set_next_task() {
  bool set = false;

  for (int i = 0; i < MAX_TASKS; i++) {
    if (!tp[i].finished && tp[i].alive) {
      current_task = i;
      task_activate(i);
      set = true;
      break;
    }
  }
  if (!set) {
    /* Check idle time */
    current_task = -1;
    printf("%ld: CPU Idle time\n", get_systime(MILLI));
  }
}

long get_systime(int unit) {
  struct timespec t;
  long tu, mul, div;

  /* Choose the values to get based on the desired unit */
  switch (unit) {
  case MICRO:
    mul = 1000000;
    div = 1000;
    break;
  case MILLI:
    mul = 1000;
    div = 1000000;
    break;
  default:
    mul = 1000;
    div = 1000000;
    break;
  }

  clock_gettime(CLOCK_MONOTONIC, &t);
  tu = (t.tv_sec - ptask_t0.tv_sec) * mul;
  tu += (t.tv_nsec - ptask_t0.tv_nsec) / div;
  return tu;
}
