/*************************************************************
 * File: pqueue-heap.cpp
 *
 * Implementation file for the HeapPriorityQueue
 * class.
 */
 
#include "pqueue-heap.h"
#include "error.h"
using namespace std;

const int arrayStartingSize = 1;

/* Bubble-up - repeatedly swap new node with parent node until its in proper place. */
void HeapPriorityQueue::bubbleUp(){
	int curInd = realSize - 1;
	while(curInd >= 0){
		int parentInd = (curInd - 1) / 2;
		if(data[curInd] >= data[parentInd]){
			break;
		}

		swap(data[curInd], data[parentInd]);
		curInd = parentInd;
	}
}

/* Bubble-down - repeatedly swap with smaller child until it comes to rest. */
void HeapPriorityQueue::bubbleDown(){
	int curInd = 0;
	int minChildInd;
	while(curInd * 2 + 2 <= realSize){
		if(curInd * 2 + 2 < realSize){
			if(data[curInd * 2 + 1] < data[curInd * 2 + 2]){
				minChildInd = curInd * 2 + 1;
			}else {
				minChildInd = curInd * 2 + 2;
			}
		}else if(curInd * 2 + 1 < realSize){
			minChildInd = curInd * 2 + 1;
		}

		if(data[minChildInd] > data[curInd]){
			break;
		}

		swap(data[minChildInd], data[curInd]);
		int temp = minChildInd;
		curInd = temp;
	}
}

HeapPriorityQueue::HeapPriorityQueue() {
	arraySize = arrayStartingSize;
	realSize = 0;
	data = new string[arraySize]();
}

HeapPriorityQueue::~HeapPriorityQueue() {
	delete[] data;
}

int HeapPriorityQueue::size() {
	return realSize;
}

bool HeapPriorityQueue::isEmpty() {
	return realSize == 0;
}

void HeapPriorityQueue::enqueue(string value) {
	// if array is full, resize it.(double up the size)
	if(realSize == arraySize){
		string* freshData = new string[arraySize * 2];
		for(int i = 0;i < realSize; i ++){
			freshData[i] = data[i];
		}
		delete[] data;

		data = freshData;
		arraySize *= 2;
	}

	//add new value
	data[realSize++] = value;
	bubbleUp();
}

string HeapPriorityQueue::peek() {
	if(isEmpty()){
		error("Priority Queue is empty");
	}

	return data[0];
}

string HeapPriorityQueue::dequeueMin() {
	if(isEmpty()){
		error("Priority Queue is empty");
	}

	string result = data[0];
	swap(data[0], data[realSize-1]);
	realSize --;

	bubbleDown();

	return result;
}

