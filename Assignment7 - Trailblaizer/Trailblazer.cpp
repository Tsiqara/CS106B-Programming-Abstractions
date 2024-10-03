/******************************************************************************
 * File: Trailblazer.cpp
 *
 * Implementation of the graph algorithms that comprise the Trailblazer
 * assignment.
 */

#include "Trailblazer.h"
#include "TrailblazerGraphics.h"
#include "TrailblazerTypes.h"
#include "TrailblazerPQueue.h"
#include "random.h"
using namespace std;


struct Node{
	Loc location;
	Color color;
	double distance;
	Loc parent;
};

/* Function: shortestPath
 * 
 * Finds the shortest path between the locations given by start and end in the
 * specified world.	 The cost of moving from one edge to the next is specified
 * by the given cost function.	The resulting path is then returned as a
 * Vector<Loc> containing the locations to visit in the order in which they
 * would be visited.	If no path is found, this function should report an
 * error.
 *
 * In Part Two of this assignment, you will need to add an additional parameter
 * to this function that represents the heuristic to use while performing the
 * search.  Make sure to update both this implementation prototype and the
 * function prototype in Trailblazer.h.
 */
 Vector<Loc>
 shortestPath(Loc start, Loc end,
			  Grid<double>& world,
			  double costFn(Loc one, Loc two, Grid<double>& world),
			  double heuristic(Loc start, Loc end, Grid<double>& world)) {

	Vector<Loc> path;

	/* Create map with key - location and value - information about that point. */
	Map<Loc, Node> graph;
	for(int i = 0; i < world.numRows(); i ++){
		for(int j = 0;j < world.numCols(); j ++){
			Loc l;
			l.row = i;
			l.col = j;
			Node newNode;
			newNode.location = l;
			newNode.color = GRAY;
			newNode.distance = 0;

			graph.put(l, newNode);
		}
	}

	// Set color of startNode to YELLOW, and enqueue it into priority queue
	Node startNode;
	startNode.location = start;
	startNode.color = YELLOW;
	colorCell(world, startNode.location, startNode.color);
	startNode.distance = 0;
	graph[start] = startNode;

	TrailblazerPQueue<Loc> candidateNodes;
	candidateNodes.enqueue(startNode.location, heuristic(start, end, world));

	while(!candidateNodes.isEmpty()){
		// dequeue the lowest-cost node and color it GREEN
		Loc minLoc = candidateNodes.dequeueMin();

		Node curr = graph[minLoc];
		graph[minLoc].color = GREEN;
		colorCell(world, graph[minLoc].location, graph[minLoc].color);

		// if current Node is end, we have found shortest path
		if(graph[minLoc].location == end){
			Loc curLoc = graph[minLoc].location;
			while(curLoc != start){
				path.insert(0, curLoc);
				curLoc = graph[curLoc].parent;
			}
			path.insert(0, start);
			break;
		}

		// for each neighbour of current Node:
		for(int row = -1; row <= 1; row ++){
			for(int col = -1; col <= 1; col ++){
				if(row == 0 && col == 0){
					continue;
				}

				Loc nextLoc;
				nextLoc.row = curr.location.row + row;
				nextLoc.col = curr.location.col + col;
				if(world.inBounds(nextLoc.row, nextLoc.col)){
					// If neighbour is GRAY, color it YELLOW, set it's parent to current Node(save it's location) and enqueue into priority queue.
					if(graph[nextLoc].color == GRAY){
						graph[nextLoc].color = YELLOW;
						colorCell(world, nextLoc, YELLOW);
						graph[nextLoc].distance = curr.distance + costFn(curr.location, nextLoc, world);
						graph[nextLoc].parent = curr.location;
						candidateNodes.enqueue(graph[nextLoc].location, graph[nextLoc].distance + heuristic(nextLoc, end, world));
					}else if(graph[nextLoc].color == YELLOW && graph[nextLoc].distance > curr.distance + costFn(curr.location, nextLoc, world)){
						// If neightbour is YELLOW, it means we have already visited it, so if we have found path with less cost we should update it.
						graph[nextLoc].distance = curr.distance + costFn(curr.location, nextLoc, world);
						graph[nextLoc].parent = curr.location;
						candidateNodes.decreaseKey(graph[nextLoc].location, graph[nextLoc].distance + heuristic(nextLoc, end, world));
					}

				}
			}
		}

	}
    return path;
}

void enqueueEdge(Loc startLoc, int endRow, int endCol, TrailblazerPQueue<Edge>& edges, Set<Edge>& alreadyEnqueued){
	Loc endLoc;
	endLoc.row = endRow;
	endLoc.col = endCol;
	Edge newEdge = makeEdge(startLoc, endLoc);
	if(!alreadyEnqueued.contains(newEdge)){
		edges.enqueue(newEdge, randomReal(0,1));
		alreadyEnqueued.add(newEdge);
	}
 }

Set<Edge> createMaze(int numRows, int numCols) {
	Set<Edge> maze;

	//create and enqueue into priority queue all possible edges
	TrailblazerPQueue<Edge> edges;
	Set<Edge> alreadyEnqueued;
	for(int i = 0; i < numRows; i ++){
		for(int j = 0; j < numCols; j ++){
			Loc startLoc;
			startLoc.row = i;
			startLoc.col = j;

			Loc endLoc;
			if(i + 1 < numRows){
				enqueueEdge(startLoc, i + 1, j, edges, alreadyEnqueued);
			}
			if(i - 1 >= 0){
				enqueueEdge(startLoc, i - 1, j, edges, alreadyEnqueued);
			}
			if(j + 1 < numCols){
				enqueueEdge(startLoc, i, j + 1, edges, alreadyEnqueued);
			}
			if(j - 1 >= 0){
				enqueueEdge(startLoc, i, j - 1, edges, alreadyEnqueued);
			}
		}
	}

	// Create clusters. If clusters[i] == clusters[j] it means they are in the same cluster
	int numClusters = numRows * numCols;
	Vector<int> clusters;

	for(int i = 0; i < numClusters; i ++){
		clusters.add(i);
	}

	while(numClusters >= 2){
		Edge curEdge = edges.dequeueMin();
		/* If start and end of edge are not in the same cluster, merge clusters and add edge into maze */
		if(clusters[curEdge.start.row * numCols + curEdge.start.col] != clusters[curEdge.end.row * numCols + curEdge.end.col]){
			int minimum = min(clusters[curEdge.start.row * numCols + curEdge.start.col], clusters[curEdge.end.row * numCols + curEdge.end.col]);
			for(int i = 0; i < clusters.size(); i ++){
				if((clusters[i] == clusters[curEdge.start.row * numCols + curEdge.start.col]) || (clusters[i] == clusters[curEdge.end.row * numCols + curEdge.end.col])){
					if(i != curEdge.start.row * numCols + curEdge.start.col && i != curEdge.end.row * numCols + curEdge.end.col){
						clusters[i] = minimum;
					}
				}
			}
			clusters[curEdge.start.row * numCols + curEdge.start.col] = minimum;
			clusters[curEdge.end.row * numCols + curEdge.end.col] = minimum;
			numClusters --;
			maze.add(curEdge);
		}
	}
    return maze;
}
