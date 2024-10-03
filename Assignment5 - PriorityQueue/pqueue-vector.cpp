/*************************************************************
 * File: pqueue-vector.cpp
 *
 * Implementation file for the VectorPriorityQueue
 * class.
 */
 
#include "pqueue-vector.h"
#include "error.h"

int VectorPriorityQueue::findMinIndex(){
	int minInd = 0;
	for(int i = 0; i < data.size(); i ++){
		if(data[i] < data[minInd]){
			minInd = i;
		}
	}

	return minInd;
}

VectorPriorityQueue::VectorPriorityQueue() {
}

VectorPriorityQueue::~VectorPriorityQueue() {
}

int VectorPriorityQueue::size() {
	return data.size();
}

bool VectorPriorityQueue::isEmpty() {
	return data.isEmpty();
}

void VectorPriorityQueue::enqueue(string value) {
	data.add(value);
}

string VectorPriorityQueue::peek() {
	if(data.isEmpty()){
		error("Priority Queue is empty");
	}
	
	return data[findMinIndex()];
}

string VectorPriorityQueue::dequeueMin() {
	if(data.isEmpty()){
		error("Priority Queue is empty");
	}
	
	int minInd = findMinIndex();
	string min = data[minInd];
	data.remove(minInd);
	return min;
}

