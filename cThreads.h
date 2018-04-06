/*****************************************************
** Author: James Meehan (Group 29)
** Description: The cThreads class creates and 
** manages the threads for the christofides algorithm
** implementation.
******************************************************/
#ifndef CTHREADS_H
#define CTHREADS_H

#include <iostream>
#include <cstring>
#include "tsp.h"
using namespace std;

class Thread
{
private:
// www.stackoverflow.com/questions/30531017/working-around-the-static-type-property-of-c?noredirect=1&lq=1
// Global function for pthraed_create and calls member function run
	static void *exec(void *thr);
	pthread_t _id;
protected:
	virtual void run() = 0;
public:
	TSP *mytsp;
	int start_node;
	Thread() {
		start_node = -1;
		mytsp = NULL;
		_id = 0;
		thread_id = -1;
	};
	void create();
	void join();
	long get_tid() {return (long)_id;}
	virtual ~Thread(){};
	int thread_id;
};


#endif
