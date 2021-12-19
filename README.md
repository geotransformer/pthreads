# pthreads

## Multithreading in Linux

The POSIX thread (pthread) libraries are a standards based thread API for C/C++. 


## Thread life cycle [] (http://www.qnx.com/developers/docs/qnxcar2/index.jsp?topic=%2Fcom.qnx.doc.neutrino.sys_arch%2Ftopic%2Fkernel_Life_Cycle.html)
The number of threads within a process can vary widely, with threads being created and destroyed dynamically.

Thread creation (pthread_create()) involves allocating and initializing the necessary resources within the process's address space (e.g., thread stack) and starting the execution of the thread at some function in the address space.

Thread termination (pthread_exit(), pthread_cancel()) involves stopping the thread and reclaiming the thread's resources. As a thread executes, its state can generally be described as either "ready" or "blocked." More specifically, it can be one of the following:

![Thread life cycle](references/pthread.jpg)

