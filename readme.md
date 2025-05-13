This needs to be run in linux with sudo privileges to run properly. Enter in the command:
```
gcc ptask.c timespec_helpers.c executive.c -o executive -lpthread
```
to build an executable called executive. Then run the command:
```
sudo ./executive
```
And the output of the Non-preemptive RM scheduler should be shown. If you want to try different inputs or task values you can simply add tasks to the main function using:
```
int task_create(void *(*task)(void *), int index, int period, int deadline,
                int priority, long wcet, int act_flag);
```
where the parameters are as follows,
  - Task function, in this case simply periodic_task
  - Task ID, should be simply starting at zero and incrementing
  - Task Period
  - Task Deadline
  - Task Priority
  - Task Worst Case Execution Time
  - Act_flag, to start the function immediately or not, which we want to fill with the macro NON_ACT as the scheduler will choose when to activate for us

NOTE: The scheduler chooses with the mindset that they are in order of priority and should be entered as such, since the logic for RM scheduling has been completed by another project I didn't feel it was necessary to spend time implementing that logic.
