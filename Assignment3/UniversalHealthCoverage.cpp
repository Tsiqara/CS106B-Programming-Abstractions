/*
 * File: UniversalHealthCoverage.cpp
 * ----------------------
 * Name: [TODO: enter name here]
 * Section: [TODO: enter section leader here]
 * This file is the starter project for the UniversalHealthCoverage problem
 * on Assignment #3.
 * [TODO: extend the documentation]
 */
#include <iostream>
#include <string>
#include "set.h"
#include "vector.h"
#include "console.h"
#include "simpio.h"
using namespace std;

/* Function: canOfferUniversalCoverage(Set<string>& cities,
 *                                     Vector< Set<string> >& locations,
 *                                     int numHospitals,
 *                                     Vector< Set<string> >& result);
 * Usage: if (canOfferUniversalCoverage(cities, locations, 4, result)
 * ==================================================================
 * Given a set of cities, a list of what cities various hospitals can
 * cover, and a number of hospitals, returns whether or not it's
 * possible to provide coverage to all cities with the given number of
 * hospitals.  If so, one specific way to do this is handed back in the
 * result parameter.
 */
bool canOfferUniversalCoverage(Set<string>& cities,
                               Vector< Set<string> >& locations,
                               int numHospitals,
                               Vector< Set<string> >& result);
/* This is a wrapper function for canOfferUniversalCoverage. 
*  It has additional parameter currIndex to help us generate subsets of locations
*  to find if there is subset of numHospitals size that can cover all cities.
*/
bool canOfferUniversalCoverageWrapper(Set<string>& cities, Vector< Set<string> >& locations, int numHospitals,
									  Vector< Set<string> >& result, int currIndex);

/* Function tells if given list of sets of cities result can cover all cities in Set<string> cities. */
bool canCover(Set<string>& cities, Vector<Set<string> >& result);

/* This is function to fill set of cities. */
void enterCities(Set<string>& cities);
/* This is function to fill lists of what cities various hospitals can cover. */
void enterLocations(Vector< Set<string> >& locations);

int main() {
    Set<string> cities;
	enterCities(cities);
	Vector< Set<string> > locations;
	enterLocations(locations);
	int numHospitals = getInteger("Enter number of hospitals: ");
	Vector< Set<string> > result; 

	if(canOfferUniversalCoverage(cities, locations, numHospitals, result)){
		cout << "Can offer universal coverage: " << endl;
		cout << result.toString() << endl;
	}else{
		cout << "No " << numHospitals << " hospitals can offer universal coverage." << endl;
	}

    return 0;
}

bool canOfferUniversalCoverage(Set<string>& cities, Vector< Set<string> >& locations, int numHospitals, Vector< Set<string> >& result){
	return canOfferUniversalCoverageWrapper(cities, locations, numHospitals, result, 0);
}

bool canOfferUniversalCoverageWrapper(Set<string>& cities, Vector< Set<string> >& locations, int numHospitals, Vector< Set<string> >& result, int currIndex){
	/* Base cases 1:
	*  numHospitals is at least size of locations. If locations cover all cities that universal coverage can be offered. If not - no.
	*/
	if(numHospitals >= locations.size()){
		if(canCover(cities, locations)){
			result = locations;
			return true;
		}
		return false;
	}
	/* Base case 2: We have generated subset of locations vector with size of numHospitals,
	* so we have chosen numHospitals hospitals from locations. If they can cover all cities, we can offer universal coverage
	* and this subset is result.
	*/
	if(result.size() == numHospitals && canCover(cities, result)){
		return true;
	}

	/* Base case 3: If currIndex is greater than locations.size()-1, we have reached end of locations vector, so have generated all subsets.
	* If we are in this if clause, it means we have not found necessary subset and answer is false.
	*/
	if(currIndex > locations.size() - 1){
		return false;
	}

	/* Recursive decomposition:
	*  We check if we can get universal coverage if we do not include locations[currIndex] set. 
	*  If we cannot, then we check if we can get it including locations[currIndex].
	*/
	bool canCoverWithoutCurrent = canOfferUniversalCoverageWrapper(cities, locations, numHospitals, result, currIndex+1);
	if(canCoverWithoutCurrent){
		return true;
	}

	result.add(locations[currIndex]);
	bool canCoverWithCurrent = canOfferUniversalCoverageWrapper(cities, locations, numHospitals, result, currIndex+1);
	if(canCoverWithCurrent){
		return true;
	}else{
		result.remove(result.size()-1);
		return false;
	}
}

bool canCover(Set<string>& cities, Vector<Set<string> >& result){
	Set<string> covered;
	for(int i = 0; i < result.size(); i ++){
		foreach(string city in result[i]){
			if(!covered.contains(city)){
				covered.add(city);
			}
		}
	}
	return cities == covered;
}

void enterCities(Set<string>& cities){
	cities += "A", "B", "C", "D", "E", "F";
}

void enterLocations(Vector< Set<string> >& locations){
	Set<string> s; 
	s += "A", "B", "C";
	locations.add(s);
	s.clear();
	s += "A", "C", "D";
	locations.add(s);
	s.clear();
	s += "B", "F";
	locations.add(s);
	s.clear();
	s += "C", "E", "F";
	locations.add(s);  

	/*Set<string> s1;
	s1.add("A"); 
	s1.add("B"); 
	s1.add("C");
	Set<string> s2; 
	s2.add("A");
	s2.add("E");
	Set<string> s3;
	s3.add("A"); 
	s3.add("D");
	Set<string> s4; 
	s4.add("A"); 
	s4.add("D");
	s4.add("C");
	Set<string> s5;
	s5.add("D");
	s5.add("B");
	s5.add("C");
	locations.add(s1); 
	locations.add(s2);
	locations.add(s3);
	locations.add(s4);
	locations.add(s5); */
}
