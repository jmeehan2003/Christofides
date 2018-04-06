/*******************************************************
** Author: James Meehan (Group 29)
** Description: This is the header file for the runThread 
** class, which run threads from cThreads.cpp TSP class
*******************************************************/
#ifndef RUNTHREAD_H
#define RUNTHREAD_H
#include "cThreads.h"

class runThread: public Thread
{
public:
	virtual void run();
};

#endif
