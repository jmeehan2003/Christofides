/********************************************************************
** Author: James Meehan
** Course: CS325 - Intro to Algorithms
** Date: 3/13/2018
** Description: Traveling Salesman Final Project.  The Christofides 
** algorithm is being implemented to solve various TSP test cases.  The
** goal is to solve the sample test cases within a 1.25x ratio to the
** optimal soluation in less than three minutes.
** Sources used to create this code:
** https://www.geeksforgeeks.org/greedy-algorithms-set-5-prims-minimum-spanning-tree-mst-2/
** http://www.graph-magics.com/articles/euler.php
** https://en.wikipedia.org/wiki/2-opt
** www.stackoverflow.com/questions/30531017/working-around-the-static-type-property-of-c?noredirect=1&lq=1
** http://www.bogotobogo.com/cplusplus/multithreading_win32.php
***********************************************************************/

#include <iostream>
#include <climits>
#include <chrono>
#include "tsp.h"
#include "twoOpt.h"
#include "runThread.h"		
using namespace std::chrono;
using tclock = std::chrono::steady_clock;

int incrementNode(int numCities);

int main(int argc, char** argv) {
	// validate usage
	if (argc < 2)
	{
		cout << "Usage: ./tsp [filename]";
		exit(1);
	}

	// setup input and output files
	string testfile;
	testfile = argv[1];
	string outFile = argv[1];
	outFile.append(".tour");
	
	TSP tsp(testfile, outFile);
	int numCities = tsp.nodeCount;
	
	// start timing
	clock_t start, end;
	start = clock();
	tclock::time_point startTimer = tclock::now();

	// get city data into struct	
	tsp.getCities();
	cout << "Number of cities: " << numCities << endl;

	// find MST using Prim's algorithm
	tsp.getMST();
	
	// use greedy algorithm to approximate perfect matching
	tsp.approxPerfectMatching();

	// Create array of thread objects
	runThread runs[1];
		
	int bestRoute = INT_MAX;
	int index, increment;

	// Amount to increment starting node by each time
	increment = incrementNode(numCities);

	// run threads for different index nodes and store paths
	// set starting point for first node
	int node = 0;
	if (numCities > 4000){node = 1000;}

	int threadID = 0;
	int remain = numCities;	
	while (remain >= increment && node < numCities) {	
		for (long i = 0; i < 1; i++) {
			runs[i].start_node = node;
			runs[i].thread_id = threadID;
			runs[i].mytsp = &tsp;
			runs[i].create();
			runs[i].join();
			node += increment;
			threadID++;
			tclock::time_point endTimer = tclock::now();
			duration<long> time_elapsed = duration_cast<duration<long>>(endTimer - startTimer);
			if (time_elapsed.count() >= 140 && numCities > 2000){
				cout << "Time is running out!!!" << endl;
				remain = 0;
				break;
			}
			else if (time_elapsed.count() > 153) {
				remain = 0;
				break;
			}
		}

		remain -=  increment;
	}

	cout << "Last threadID: " << threadID << endl;
	// Loop through each index used and find shortest path
	for (long i = 0; i < threadID; i++) {
		if (tsp.path[i][1] < bestRoute) {
			index = tsp.path[i][0];
			bestRoute = tsp.path[i][1];
		}
	}

	// create Euler circuit from best index
	tsp.createEulerCircuit(index, tsp.circuit);
	
	// create Hamiltonian cycle from Euler circuit
	tsp.createHamCycle(tsp.circuit, tsp.pathLength);

	// optimize with twoOpt
	for (int i = 0; i < 12; i++)
	{
		twoOpt(tsp.graph, tsp.circuit, tsp.pathLength, numCities);
	}
	
	end = clock();
	cout << "Time to complete christo algo: " << ((float)(end - start)) / CLOCKS_PER_SEC << "s\n";
	
	//create tour to output values to outfile	
	//tsp.createTour(index);

	// print results
	cout << "Best solution found = " << bestRoute << endl;
	cout << "\nFinal length: " << tsp.pathLength << " Best Index: " << index << endl;
	clock_t real_end = clock();
	cout << "Total time is " <<((float)(real_end - start)) / CLOCKS_PER_SEC << endl;

	// Print to file
	tsp.writeOutfile();

	return 0;
}

/*********************************************************
** Description: the increment function set the amount to
** increase the starting node by each time.
**********************************************************/
int incrementNode(int n) {
	int increment;
	if (n < 600)
		increment = 1;
	else if (n >= 600 && n < 1500)
		increment = 3;
	else if (n >= 1500 && n < 3001)
		increment = 15; 		
	else if (n >= 3001 && n < 4001)
		increment = 50; 		
	else if (n >= 4001 && n < 5001)
		increment = 120;		
	else if (n >= 5001 && n < 7501)
		increment = 250;		
	else
		increment = 500;
	
	return increment;
}

