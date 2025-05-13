/** Created for research project by Zachary Lee using the textbook for support
 * in the ptask library **/
#include "timespec_helpers.h"

#include <pthread.h>
#include <sched.h>
#include <semaphore.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define MAX_TASKS (50U)
#define ACT (1U)
#define NON_ACT (0U)
#define PREEMPTIVE (1U)

struct task_par {
  int arg;            // task index
  long wcet;          // worst case executio time
  int period;         // task period
  int deadline;       // relative deadline
  int priority;       // task priority
  int act_flag;       // activation flag
  struct timespec at; // next activation time
  struct timespec dl; // absolute deadline
  int dmiss;          // deadline miss counter
  sem_t asem;         // activation semaphore
  pthread_t tid;      // thread identifier
  bool finished;
  bool alive;
};

void ptask_init(int policy);

/**
 * @brief
 *
 * @param task Task function
 * @param index
 * @param period
 * @param deadline
 * @param priority
 * @param act_flag
 * @return
 */
int task_create(void *(*task)(void *), int index, int period, int deadline,
                int priority, long wcet, int act_flag);

void task_activate(int id);

void wait_for_activation(int id);

void wait_for_period(int id);

void wait_for_task_end(int id);

int deadline_miss(int id);

int get_task_index(void *arg);

int task_get_period(int id);

int task_get_deadline(int id);

void task_get_atime(int id, struct timespec *at);

void task_get_adline(int id, struct timespec *ad);

void task_set_period(int id, int period);

void task_set_deadline(int id, int deadline);

void task_set_next_task();

/**
 * @brief Function to get the current system time
 *
 * @param unit The time unit for system time
 */
long get_systime(int unit);
