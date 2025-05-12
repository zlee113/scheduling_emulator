#include <time.h>

#define MICRO 1
#define MILLI 2

/**
 * @brief Function for copying a time structure into another one
 *
 * @param td Destination
 * @param ts Source
 */
void time_copy(struct timespec *td, struct timespec ts);

/**
 * @brief Function for comparing two time structures
 *
 * @param t1 timespec one
 * @param t2 timespec two
 * @return 1 if t1 > t2, -1 if t2 > t1, and 0 if t1 == t2
 */
int time_cmp(struct timespec t1, struct timespec t2);

/**
 * @brief Function for adding milliseconds to a timespec
 *
 * @param t Timespec being added to
 * @param ms Value in milliseconds to add to timespec
 */
void time_add_ms(struct timespec *t, int ms);


