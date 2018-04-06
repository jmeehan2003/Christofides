/*****************************************
** Author: James Meehan
** Description: runThread runs threads from
** cThreads.cpp
*****************************************/

#include "runThread.h"

void runThread::run()
{
	int path = mytsp->getBestPath(start_node);

	mytsp->path[thread_id][0] = start_node;
	mytsp->path[thread_id][1] = path;

	pthread_exit(NULL);
}
