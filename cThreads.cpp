/************************************************************************
** Author: James Meehan (Group 29)
** Desciption: This is the cThreads class implementation file for
** the Christofides algorithm. 
************************************************************************/
#include "cThreads.h"

/************************************************************************
** Description:  The create() function creates new threads
************************************************************************/
void Thread::create() {
	int n;
	if ((n = pthread_create(&_id, NULL, &Thread::exec, this)) != 0) {
		cout << "Error in creating thread" << endl;
	}
}

/************************************************************************
** Description:  The join() function lets threads join and waits for 
**termination status
************************************************************************/
void Thread::join() {
	void *stat;
	int n = pthread_join(_id, &stat);
	if (n) {
		cout << "Error when thread " << thread_id << " joined.\n";
		exit(1);
	}
}

/************************************************************************
** This function executes the run command in the subclass
** RunThread.  The code was adapted from:
** http://www.bogotobogo.com/cplusplus/multithreading_win32.php
************************************************************************/
void *Thread::exec(void *thr) {
	reinterpret_cast<Thread *>(thr)->run();
	return NULL;
}

