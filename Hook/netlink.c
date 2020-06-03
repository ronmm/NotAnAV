#include "netlink.h"

#define NETLINK_USER 31

struct sock *nl_sk = NULL;

void send_msg_to_kernel(struct sk_buff *skb)
{
    struct nlmsghdr *nlh;
    int pid;

    printk(KERN_INFO
    "Entering: %s\n", __FUNCTION__);

    nlh = (struct nlmsghdr *) skb->data;
    printk(KERN_INFO "Netlink received msg payload: %s\n", (char *) nlmsg_data(nlh));
    pid = nlh->nlmsg_pid; /*pid of sending process */
    printk(KERN_INFO "Netlink received pid: %d\n", pid);

}

void send_msg_from_kernel(char *total_size) {
    struct nlmsghdr *nlh;
    struct sk_buff *skb_out;
    int msg_size;
    char *msg = "Hello from kernel";
    int res;
    int pid = 0;
    msg_size = strlen(total_size);
    skb_out = nlmsg_new(msg_size, 0);

    if (!skb_out)
    {

        printk(KERN_ERR "Failed to allocate new skb\n");
        return;

    }
    nlh = nlmsg_put(skb_out, 0, 0, NLMSG_DONE, msg_size, 0);
    NETLINK_CB(skb_out).dst_group = 0; /* not in mcast group */
    strncpy(nlmsg_data(nlh), total_size, msg_size);

    struct task_struct *task;

    for_each_process(task) {

        /* compare your process name with each of the task struct process name*/

        if ( (strcmp( task->comm,"hokk") == 0 ) ) {

            /* if matched that is your user process PID */
            pid = task->pid;
        }
    }

    if (pid != 0) {
        res = nlmsg_unicast(nl_sk, skb_out, pid);
    }
    else {
        res = 0;
    }

    if (res < 0)
        printk(KERN_INFO "Error while sending back to user\n");
}

struct netlink_kernel_cfg cfg = {
        .groups  = 1,
        .input = send_msg_to_kernel,
};

int open_netlink(void)
{
    printk("Entering: %s\n", __FUNCTION__);
    nl_sk = netlink_kernel_create(&init_net, NETLINK_USER, &cfg);

    if (!nl_sk)
    {
        printk(KERN_ALERT "Error creating socket.\n");
        return -10;
    }

    return 0;
}

void release_netlink(void)
{
    printk(KERN_INFO "exiting hello module\n");
    netlink_kernel_release(nl_sk);
}