/*
 * File: Sierpinski.cpp
 * --------------------------
 * Name: [TODO: enter name here]
 * Section: [TODO: enter section leader here]
 * This file is the starter project for the Sierpinski problem
 * on Assignment #3.
 * [TODO: extend the documentation]
 */

#include <iostream>
#include "math.h"
#include "simpio.h"
#include "gwindow.h"
using namespace std;

void drawSiepinskiTriangle(GWindow& gw, double length, int order);
void drawSierpinskiTriangleWrapper(GWindow& gw, GPoint p1, GPoint p2, GPoint p3, int order);

int main() {
	GWindow gw;
	double length = getReal("Enter length of triangle edge: ");
	int order = getInteger("Enter fractal order for Sierpinski Triangle: ");
	drawSiepinskiTriangle(gw, length, order); 
	return 0;
}

/* Draw Sierpinski Triangle of 0-order and 
* pass GPoints of it's vertices to drawSierpinskiTriangleWraper along with GWindow&gw and sierpinski triangle order.
*/
void drawSiepinskiTriangle(GWindow& gw, double length, int order){
	GPoint p1((gw.getWidth() - length) / 2, gw.getHeight()/2 + length * sqrt(3.0) / 6);
	GPoint p2 = gw.drawPolarLine(p1, length, +60);
	GPoint p3 = gw.drawPolarLine(p2, length, -60);
	gw.drawLine(p3, p1);
	drawSierpinskiTriangleWrapper(gw, p1, p2, p3, order);
} 

/* Connects midpoints of edges of triangle with vertices p1,p2,p3. */
void drawSierpinskiTriangleWrapper(GWindow& gw, GPoint p1, GPoint p2, GPoint p3, int order){
	//Base case: we have already drawn o-order sierpinski triangle.
	if(order == 0){
		return;
	}
	/* Recursive decomposition: calculate coordinates of midpoints of drawn lines. 
	*  call drawSierpinskiTriangleWrapper 3 times for produced small triangles.
	*/
	GPoint pt1((p1.getX() + p2.getX())/2, (p1.getY() + p2.getY())/2);
	GPoint pt2((p3.getX() + p2.getX())/2, (p3.getY() + p2.getY())/2);
	GPoint pt3((p1.getX() + p3.getX())/2, (p1.getY() + p3.getY())/2);
	gw.drawLine(pt1, pt2);
	gw.drawLine(pt2, pt3);
	gw.drawLine(pt1, pt3);
	order --;
	drawSierpinskiTriangleWrapper(gw, p1, pt1, pt3, order);
	drawSierpinskiTriangleWrapper(gw, pt1, p2, pt2, order);
	drawSierpinskiTriangleWrapper(gw, pt3, pt2, p3, order);
}