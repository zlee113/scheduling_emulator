#include "timespec_helpers.h"

void time_copy(struct timespec *td, struct timespec ts) {
  /* Copy ts values into td */
  td->tv_sec = ts.tv_sec;
  td->tv_nsec = ts.tv_nsec;
}

int time_cmp(struct timespec t1, struct timespec t2) {
  /* Check if t1 is bigger in the seconds field */
  if (t1.tv_sec > t2.tv_sec)
    return 1;
  /* Check if t2 is bigger in the seconds field */
  if (t1.tv_sec < t2.tv_sec)
    return -1;
  /* If the same seconds field check if t1 nano is bigger */
  if (t1.tv_nsec > t2.tv_nsec)
    return 1;
  /* If the same seconds field check if t2 nano is bigger */
  if (t1.tv_nsec < t2.tv_nsec)
    return -1;
  /* If same value for each return 0 */
  return 0;
}

void time_add_ms(struct timespec *t, int ms) {
  /* Add seconds and nanoseconds */
  t->tv_sec += ms / 1000;
  t->tv_nsec += (ms % 1000) * 1000000;

  /* Check if the nanosecond has breached a full second */
  if (t->tv_nsec > 1000000000) {
    t->tv_nsec -= 1000000000;
    t->tv_sec += 1;
  }
}
