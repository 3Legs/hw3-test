#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <fcntl.h>

#define __NR_net_lock 333
#define __NR_net_unlock 334
#define __NR_net_lock_wait_timeout 335

enum __netlock_t {
   NET_LOCK_USE,
   NET_LOCK_SLEEP
};
typedef enum __netlock_t netlock_t;

void get_sys_time(int *sec, int *milli)
{
	struct timeval *temp = malloc(sizeof(struct timeval));
	gettimeofday(temp, NULL);
	*sec = temp->tv_sec;
	*milli = temp->tv_usec;
}

int main(int argc, char **argv)
{
	int sec, usec;
	int result;
	pid_t pid = getpid();
	while(1) {
		get_sys_time(&sec, &usec);
		printf("Sleeper called net_lock at sec: %d usec: %d, sleeper pid: %d\n", sec, usec, pid);
		result = syscall(__NR_net_lock, NET_LOCK_SLEEP, 1);
		printf("Sleeper called net_lock_wait_timeout at sec: %d usec: %d, sleeper pid: %d\n", sec, usec, pid);
		result = syscall(__NR_net_lock_wait_timeout);
		printf("Sleeper called net_unlock at sec: %d usec: %d, sleeper pid: %d\n", sec, usec, pid);
		result = syscall(__NR_net_unlock);
	}
}