/*
 * File: InverseGenetics.cpp
 * --------------------------
 * Name: [TODO: enter name here]
 * Section: [TODO: enter section leader here]
 * This file is the starter project for the Inverse Genetics problem
 * on Assignment #3.
 * [TODO: extend the documentation]
 */

#include <iostream>
#include <string>
#include <fstream>
#include "set.h"
#include "map.h"
#include "console.h"
#include "simpio.h"
using namespace std;

/* Function: listAllRNAStrandsFor(string protein,
 *                                Map<char, Set<string> >& codons);
 * Usage: listAllRNAStrandsFor("PARTY", codons);
 * ==================================================================
 * Given a protein and a map from amino acid codes to the codons for
 * that code, lists all possible RNA strands that could generate
 * that protein
 */
void listAllRNAStrandsFor(string protein, Map<char, Set<string> >& codons);

/* This function given a protein and a map from amino acid codes to the codons for that code, 
*  returns set of all possible RNA strands that could generate that protein.
*/
Set<string> getAllRNAStrandsFor(string protein, Map<char, Set<string> >& codons);

/* Function: loadCodonMap();
 * Usage: Map<char, Lexicon> codonMap = loadCodonMap();
 * ==================================================================
 * Loads the codon mapping table from a file.
 */
Map<char, Set<string> > loadCodonMap();

int main() {
    /* Load the codon map. */
    Map<char, Set<string> > codons = loadCodonMap();

	string protein = getLine("Enter protein: ");
	listAllRNAStrandsFor(protein, codons);

    return 0;
}

void listAllRNAStrandsFor(string protein, Map<char, Set<string> >& codons){
	Set<string> strandsSet = getAllRNAStrandsFor(protein, codons);
	cout << "Strands that code protein " << protein << " are: " << endl;
	foreach(string strand in strandsSet){
		cout << strand << endl;
	}
}

Set<string> getAllRNAStrandsFor(string protein, Map<char, Set<string> >& codons){
	/* If protein.size() == 0, means it is one amino acid and all combinations that code it are in the map. */
	if(protein.size() == 1){
		return codons[protein[0]];
	}

	/* Recursive decomposition:
	*  Save in currentSet all combinations for first amino acid and  generate all RNA strands for protein without first amino acid.
	*  then concatenate each combinations of currSet and each strand from  all RNA strands for protein without first amino acid.
	*/
	Set<string> strandsSet;
	Set<string> currSet = codons[protein[0]];
	foreach(string codon in currSet){
		foreach(string strand in getAllRNAStrandsFor(protein.substr(1), codons)){
			strandsSet += (codon + strand);

		}
	}
	return strandsSet;
}

/* You do not need to change this function. */
Map<char, Set<string> > loadCodonMap() {
    ifstream input("codons.txt");
    Map<char, Set<string> > result;

    /* The current codon / protein combination. */
    string codon;
    char protein;

    /* Continuously pull data from the file until all data has been
     * read.
     */
    while (input >> codon >> protein) {
        result[protein] += codon;
    }

    return result;
}
