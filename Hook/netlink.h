#include <linux/module.h>
#include <net/sock.h>
#include <linux/netlink.h>
#include <linux/skbuff.h>
#include <net/net_namespace.h>

void send_msg_to_kernel(struct sk_buff *skb);
void send_msg_from_kernel(char *total_size);
int open_netlink(void);
void release_netlink(void);
