#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <linux/netlink.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <limits.h>
#include <errno.h>


typedef struct {
    int iPid;
    int iTid;
} Thread;

#define init_module(module_image, len, param_values) syscall(__NR_init_module, module_image, len, param_values)
#define delete_module(name, flags) syscall(__NR_delete_module, name, flags)

int open_netlink(void);
void read_event(int sock);
int checkType(char* in);
Thread* parseThreads(char* buffer);
Thread* getUserThreads(void);
int* parseProcesses(char* buffer);
int* getUserProcesses(void);
int* getUserNetwork(void);
int* getUserModules(void);
void compareProc(int* procK, int* procU);
void compareThreads(Thread* threadK, Thread* threadU);
void sendOverSocket(char* data, char* tag);
void take_dump();
int* getSysModules(void);


