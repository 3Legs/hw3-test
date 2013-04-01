#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
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

int get_num(int i)
{
	int ret = 1;
	int p = 0;
	for(p = 0; p < i; ++p) {
		ret *= 10;
	}
	return ret;
}

int parse_int(char *name)
{
	int len = strlen(name);
	int ret = 0;
	int i = 0;
	for(i = 0; i < len; ++i) {
		if(name[i]<48 || name[i]>57) {
			return -1;
		}
	}
	for(i = 0; i < len; ++i) {
		ret += (int)(name[i]-48) * get_num(len-i-1);
	}
	return ret;
}

int main(int argc, char **argv) 
{
	int sec = 0;
	int usec = 0;
	int result = 0;
	int i = 0;
	char *arg1 = NULL;
	char *arg2 = NULL;
	char *arg3 = NULL;
	int int1 = 0;
	int int2 = 0;
	int int3 = 0;
	pid_t pid = getpid();
	arg1 = argv[1];
	arg2 = argv[2];
	arg3 = argv[3];
	if(arg1 == NULL || arg2 == NULL || arg3 == NULL) {
		printf("my_net_app must take three arguments! Abort!\n");
		return -1;
	}
	int1 = parse_int(arg1);
	int2 = parse_int(arg2);
	int3 = parse_int(arg3);
	if(int1 == -1 || int2 == -1 || int3 == -1) {
		printf("The first three arguments must be integers! Abort!\n");
		return -1;
	}
	get_sys_time(&sec, &usec);
	printf("sec: %d: usec: %d sleeping pid:%d args:%d, %d\n", sec, usec, pid, int1, int2);
	sleep(int1);
	get_sys_time(&sec, &usec);
	printf("sec: %d: usec: %d calling_net_lock pid:%d\n", sec, usec, pid);
	result = syscall(__NR_net_lock, NET_LOCK_USE, int2);
	if(result < 0) {
		printf("Error occured calling net_lock!\n");
		return -1;
	}
	get_sys_time(&sec, &usec);
	printf("sec: %d: usec: %d return_net_lock pid:%d\n", sec, usec, pid);
	for(i = 0; i < int3; ++i) {}
	get_sys_time(&sec, &usec);
	printf("sec: %d: usec: %d calling_net_unlock pid:%d\n", sec, usec, pid);
	result = syscall(__NR_net_unlock);
	if(result < 0) {
		printf("Error occured calling net_unlock!\n");
		return -1;
	}
	return 0;
}