#include "netlink.h"

/* Protocol family, consistent in both kernel prog and user prog. */
#define MYPROTO NETLINK_USERSOCK
/* Multicast group, consistent in both kernel prog and user prog. */
#define MYGRP 21

struct sock *nl_sk = NULL;

void send_to_user(char *msg)
{
    struct sk_buff *skb;
    struct nlmsghdr *nlh;
    int msg_size = strlen(msg) + 1;
    int res;

    pr_info("Creating skb.\n");
    skb = nlmsg_new(NLMSG_ALIGN(msg_size + 1), GFP_KERNEL);
    if (!skb) {
        pr_err("Allocation failure.\n");
        return;
    }

    nlh = nlmsg_put(skb, 0, 1, NLMSG_DONE, msg_size + 1, 0);
    strcpy(nlmsg_data(nlh), msg);

    pr_info("Sending skb.\n");
    res = nlmsg_multicast(nl_sk, skb, 0, MYGRP, GFP_KERNEL);
    if (res < 0)
        pr_info("nlmsg_multicast() error: %d\n", res);
    else
        pr_info("Success.\n");
}

int createNetlink(void)
{
    pr_info("Inserting hello module.\n");

    nl_sk = netlink_kernel_create(&init_net, MYPROTO, NULL);
    if (!nl_sk) {
        pr_err("Error creating socket.\n");
        return -10;
    }

    //send_to_user();

    //netlink_kernel_release(nl_sk);
    return 0;
}

int releaseNetlink(void)
{
	netlink_kernel_release(nl_sk);
	return 0;
}

