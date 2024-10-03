/*************************************************************
 * File: pqueue-linkedlist.cpp
 *
 * Implementation file for the LinkedListPriorityQueue
 * class.
 */
 
#include "pqueue-linkedlist.h"
#include "error.h"

LinkedListPriorityQueue::LinkedListPriorityQueue() {
	head = new Node;
	tail = new Node;
	head->next = tail;
	tail->next = NULL;
	currentSize = 0;
}

LinkedListPriorityQueue::~LinkedListPriorityQueue() {
	Node* prevNode;
	while(head != NULL){
		prevNode = head;
		head = head-> next;
		delete prevNode;
	}
}

int LinkedListPriorityQueue::size() {
	return currentSize;
}

bool LinkedListPriorityQueue::isEmpty() {
	return (head->next == tail);
}

void LinkedListPriorityQueue::enqueue(string value) {
	Node* currentNode = head->next;
	Node* prevNode = head;

	while(currentNode != NULL){	
		if((value <= currentNode->value) || (currentNode == tail)){
			Node* newNode = new Node;
			newNode->value = value;
			newNode->next = prevNode->next;
			prevNode->next = newNode;
			currentSize += 1;
			break;
		}

		prevNode = currentNode;
		currentNode = currentNode->next;
	}
}

string LinkedListPriorityQueue::peek() {
	if(isEmpty()){
		error("Priority Queue is empty");
	}

	return head->next->value;
}

string LinkedListPriorityQueue::dequeueMin() {
	if(isEmpty()){
		error("Priority Queue is empty");
	}

	Node* currentNode = head->next;
	string result = currentNode->value;
	head->next = currentNode->next;
	delete currentNode;
	currentSize -= 1;

	return result;
}

