#include "not_an_av_process.h"

// Iterates over running processes
void getProcessesOut(char* out)
{
	struct task_struct *task;
	for_each_process(task)
	{
		char buf[sizeof(int) +(sizeof(char)*17)+1 ];
		sprintf(buf, "%d %s\n", task->pid,task->comm);
		strcat(out, buf);
	}
}

// Iterates over running threads
void getThreadsOut(char* out)
{
	struct task_struct *task;
	struct task_struct *thread;
	rcu_read_lock();

	// for_each_process_thread is for kernel 4+ and do-while is for kernel version under 4
	//for_each_process_thread(task,thread)
	do_each_thread(task, thread)
	{
		char buf[sizeof(int)*2 + sizeof(char) + 2];
		sprintf(buf, "%d %d,", task->pid, thread->pid);
		strcat(out, buf);
	} while_each_thread(task, thread);
	rcu_read_unlock();
}
