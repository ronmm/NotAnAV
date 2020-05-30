#include "user.h"
#include <unistd.h>

/* Protocol family, consistent in both kernel prog and user prog. */
#define MYPROTO NETLINK_USERSOCK
/* Multicast group, consistent in both kernel prog and user prog. */
#define MYMGRP 21

// Define constant variables
#define TCP_SERVER_IP "10.0.0.24" // TODO: Change this to the real server IP
//#define LIME_PORT 1235
#define SLEEP_INTERVAL 500000

//double dumpPeriod = 16;



int open_netlink(void)
{
    // Initialize variable
    int sock;
    struct sockaddr_nl addr;
    int group = MYMGRP;

    // Creating socket
    sock = socket(AF_NETLINK, SOCK_RAW, MYPROTO);
    if (sock < 0) {
        printf("sock < 0.\n");
        return sock;
    }

    memset((void *) &addr, 0, sizeof(addr));
    addr.nl_family = AF_NETLINK;
    addr.nl_pid = getpid();

    // Bind socket
    if (bind(sock, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
        printf("bind < 0.\n");
        return -1;
    }

    if (setsockopt(sock, 270, NETLINK_ADD_MEMBERSHIP, &group, sizeof(group)) < 0) {
        printf("setsockopt < 0\n");
        return -1;
    }

    return sock;
}

// Get the Data from kernel, Collect the data from user and send it to remote server
void read_event(int sock)
{
    // Initialize variables
    struct sockaddr_nl nladdr;
    struct msghdr msg;
    struct iovec iov;
    char buffer[65536];
    int ret;
	
    iov.iov_base = (void *) buffer;
    iov.iov_len = sizeof(buffer);
    msg.msg_name = (void *) &(nladdr);
    msg.msg_namelen = sizeof(nladdr);
    msg.msg_iov = &iov;
    msg.msg_iovlen = 1;
	// Get data from the kernel
    ret = recvmsg(sock, &msg, 0);
    if (ret < 0)
        printf("ret < 0.\n");
    else
    {
	    sprintf(buffer, "%s\n", NLMSG_DATA((struct nlmsghdr *) &buffer));
//	// Check the data type and enter the corresponding routine
//	int type = checkType(buffer);
//
//	// The data sent from kernel is processes
//	if(type == 1)
//	{
//
//		char kernProcTag[16] = "kernelProcesses";
//		// Send kernel processes to remote server
//		sendOverSocket(buffer, kernProcTag);
//		char footer[7] = "EndData";
//		sendOverSocket("", footer);
//		sleep(1);
//		// Collect and Send user processes to remote server
//		getUserProcesses();
//		char finishProc[7] = "finish";
//		char finishProcTag[19] = "not_an_av_finish_proc";
//		sendOverSocket(finishProc, finishProcTag);
//		sendOverSocket("", footer);
//
//		// Collect and Send user network to remote server
//		getUserNetwork();
//		char finishNet[7] = "finish";
//		char finishNetworkTag[17] = "not_an_av_finish_net";
//		sendOverSocket(finishNet, finishNetworkTag);
//		sendOverSocket("", footer);
//		sleep(1);
//		// Open a listener and wait for a message to see if memdump is 			needed
//        	socketForMemdump();
//		sleep(1);
//
//	}
//	// The data sent from kernel is Threads
//	if(type == 2)
//	{
//		char footer[7] = "EndData";
//		char kernThreadTag[14] = "kernelThreads";
//		// Send kernel threads to remote server
//		sendOverSocket(buffer, kernThreadTag);
//		sendOverSocket("", footer);
//		sleep(1);
//		// Collect and Send user threads to remote server
//		getUserThreads();
//		char finishThreadTag[21] = "not_an_av_finish_thread";
//		char finishThread[7] = "finish";
//		sendOverSocket(finishThread, finishThreadTag);
//		sendOverSocket("", footer);
//		sleep(1);
//	}
//	// The data sent from kernel is modules
//	if(type == 3)
//	{
//		char footer[7] = "EndData";
//		char kernModuleTag[14] = "kernelModules";
//		// Send kernel modules ro remote server
//		sendOverSocket(buffer, kernModuleTag);
//		sendOverSocket("", footer);
//		sleep(1);
//		// Collect and Send sys modules to remote server
//		getSysModules();
//		// Collect and Send user modules to remote server
//		getUserModules();
//		char finishMod[7] = "finish";
//		char finishModuleTag[17] = "not_an_av_finish_mod";
//		sendOverSocket(finishMod, finishModuleTag);
//		sendOverSocket("", footer);
//		// Open a listener and wait for a message to see if memdump is 			needed
//        	socketForMemdump();
//		sleep(1);
//	}
    }
}

// Send the collected Data to remote server
void sendOverSocket(char* data, char* tag)
{	
	int sock;
	struct sockaddr_in server;
	char server_reply[2000];
	printf("%s", tag);
	strcat(tag,data);
	//Create Socket
	usleep(SLEEP_INTERVAL);
	sock = socket(AF_INET , SOCK_STREAM , 0);
	if (sock == -1 )
	{
		printf("Could not create socket");
	}
	puts("Socket created");
	
	server.sin_addr.s_addr = inet_addr(TCP_SERVER_IP);
	server.sin_family = AF_INET;
	server.sin_port = htons (1234);

	//connect to remote server
	if(connect(sock , (struct sockaddr *)&server, sizeof(server)) < 0)
	{
		perror("connection failed, error");
		return 1;
	}
	puts("Connected\n");
	
	//keep communicating with server

	//send some data
	if( send(sock , tag , strlen(tag) , 0) < 0 )
	{
		puts("Send failed");
		return 1;
	}
		
	close(sock);
	puts("Socket Closed\n");
	return 0;
}

int main(int argc, char *argv[])
{	
    // Open netlink
    int nls;
    nls = open_netlink();
    if (nls < 0)
        return nls;

    printf("Agent started...\nChoose parameters in main file!\nPlease insert the module!\n");
    // Enter infinite loop of read_event
    for (;;)
        read_event(nls);

    return 0;
}
