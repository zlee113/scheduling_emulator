/** Created for research project by Zachary Lee using the textbook for support
 * in the ptask library **/
#include <pthread.h>
#include <semaphore.h>

#define MAX_TASKS 10

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
  pthread_t tid;        // thread identifier
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
                int priority, int act_flag);

void task_activate(int id);

void wait_for_activation(int id);

int wait_for_period(int id);

void wait_for_task_end(int id);

int deadline_miss(int id);

int get_task_index(void* arg);

void task_get_period(int id);

void task_get_period(int id);

void task_get_deadline(int id);

void task_get_atime(int id);

void task_get_adline(int id);

void task_set_period(int id);

void task_set_deadline(int id);

/**
 * @brief Function to get the current system time
 *
 * @param unit The time unit for system time
 */
long get_systime(int unit);


