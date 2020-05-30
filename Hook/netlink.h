#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/netlink.h>
#include <net/netlink.h>
#include <net/net_namespace.h>

void send_to_user(char* msg);
int createNetlink(void);
int releaseNetlink(void);