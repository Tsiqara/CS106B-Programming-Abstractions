/*************************************************************
 * File: pqueue-doublylinkedlist.cpp
 *
 * Implementation file for the DoublyLinkedListPriorityQueue
 * class.
 */
 
#include "pqueue-doublylinkedlist.h"
#include "error.h"

DoublyLinkedListPriorityQueue::Node* DoublyLinkedListPriorityQueue::findMinNode(){
	Node* minNode = head->next;
	Node* currentNode = head->next;
	while(currentNode != tail){
		if(currentNode->value < minNode->value){
			minNode = currentNode;
		}

		currentNode = currentNode->next;
	}
	return minNode;
}


DoublyLinkedListPriorityQueue::DoublyLinkedListPriorityQueue() {
	head = new Node;
	tail = new Node;
	head->prev = NULL;
	head->next = tail;
	tail->prev = head;
	tail->next = NULL;

	currentSize = 0;
}

DoublyLinkedListPriorityQueue::~DoublyLinkedListPriorityQueue() {
	Node* prevNode;
	while(head != NULL){
		prevNode = head;
		head = head->next;
		delete prevNode;
	}
}

int DoublyLinkedListPriorityQueue::size() {
	return currentSize;
}

bool DoublyLinkedListPriorityQueue::isEmpty() {
	return (head->next == tail);
}

void DoublyLinkedListPriorityQueue::enqueue(string value) {
	Node* newNode = new Node;
	newNode->value = value;
	newNode->prev = head;
	newNode->next = head->next;
	currentSize += 1;

	head->next->prev = newNode;
	head->next = newNode;
}

string DoublyLinkedListPriorityQueue::peek() {
	if(isEmpty()){
		error("Priority Queue is empty");
	}

	Node* minNode = findMinNode();
	
	return minNode->value;
}

string DoublyLinkedListPriorityQueue::dequeueMin() {
	if(isEmpty()){
		error("Priority Queue is empty");
	}

	Node* minNode = findMinNode();
	string result = minNode->value;

	minNode->prev->next = minNode->next;
	minNode->next->prev = minNode->prev;
	delete minNode;

	currentSize -= 1;

	return result;
}

