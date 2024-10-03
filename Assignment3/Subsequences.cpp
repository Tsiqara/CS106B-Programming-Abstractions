/*
 * File: Subsequences.cpp
 * ----------------------
 * Name: [TODO: enter name here]
 * Section: [TODO: enter section leader here]
 * This file is the starter project for the Subsequences problem
 * on Assignment #3.
 * [TODO: extend the documentation]
 */

#include <iostream>
#include <string>
#include "console.h"
#include "simpio.h"
using namespace std;

/* Given two strings, returns whether the second string is a
 * subsequence of the first string.
 */
bool isSubsequence(string text, string subsequence);

int main() {
	string text = getLine("Enter text: ");
	string subsequence = getLine("Enter sunsequence: ");

	if(isSubsequence(text, subsequence)){
		cout << subsequence << " is subsequence of " << text << endl;
	}else{	
		cout << subsequence << " is not subsequence of " << text << endl;
	}
    return 0;
}

bool isSubsequence(string text, string subsequence){
	/* Base case 1: empty string is subsequence of every string */
	if(subsequence == ""){
		return true;
	}
	/* Base case 2: only subsequence of empty string is empty string */
	if(text == "" && subsequence != ""){
		return false;
	} 

	/* Recursive step: 
	1)If the first letters of text and subsequence are same, we check if subsequence without first letter
	is Subsequence of text without first letter.
	2)If first letters are not the same, we check if subsequence is Subsequence of the text without first letter. */
	if(text[0] == subsequence[0]){
		return isSubsequence(text.substr(1), subsequence.substr(1));
	}else{
		return isSubsequence(text.substr(1), subsequence);
	}
}