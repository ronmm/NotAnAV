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

#define init_module(module_image, len, param_values) syscall(__NR_init_module, module_image, len, param_values)
#define delete_module(name, flags) syscall(__NR_delete_module, name, flags)

int open_netlink(void);
void read_event(int sock);
void sendOverSocket(char* data, char* tag);


