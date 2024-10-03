/*
 * File: Boggle.cpp
 * ----------------
 * Name: [TODO: enter name here]
 * Section: [TODO: enter section leader here]
 * This file is the main starter file for Assignment #4, Boggle.
 * [TODO: extend the documentation]
 */

#include <iostream>
#include "gboggle.h"
#include "grid.h"
#include "gwindow.h"
#include "lexicon.h"
#include "random.h"
#include "simpio.h"
using namespace std;

/* Constants */

const int BOGGLE_WINDOW_WIDTH = 650;
const int BOGGLE_WINDOW_HEIGHT = 350;
const int BOARD_SIZE = 16;
const int BOARD_SIDE = 4;

const string STANDARD_CUBES[16]  = {
    "AAEEGN", "ABBJOO", "ACHOPS", "AFFKPS",
    "AOOTTW", "CIMOTU", "DEILRX", "DELRVY",
    "DISTTY", "EEGHNW", "EEINSU", "EHRTVW",
    "EIOSST", "ELRTTY", "HIMNQU", "HLNNRZ"
};
 
const string BIG_BOGGLE_CUBES[25]  = {
    "AAAFRS", "AAEEEE", "AAFIRS", "ADENNN", "AEEEEM",
    "AEEGMU", "AEGMNN", "AFIRSY", "BJKQXZ", "CCNSTW",
    "CEIILT", "CEILPT", "CEIPST", "DDLNOR", "DDHNOT",
    "DHHLOR", "DHLNOR", "EIIITT", "EMOTTT", "ENSSSU",
    "FIPRSY", "GORRVW", "HIPRRY", "NOOTUW", "OOOTTU"
};

struct coordinateT{
	int row;
	int col;
};

/* Function prototypes */

void welcome();
void giveInstructions();
bool giveUserChanceToEnterCubeConfigurations(Vector<string>& cubes);
void chooseSideAndFillTheGrid(Grid<char>& board, Vector<string>& cubes);
void shuffleCubes(Vector<string>& cubes);
void arrangeCubesOnBoard(Grid<char>& board, Vector<string>& cubes, bool isUserConfiguration);
void labelCubes(Grid<char>& board);
bool vectorContains(Vector<coordinateT>& path, int row, int col);
bool canBeConnected(Grid<char>& board, char nextChar, int curRow, int curCol, int& nextRow, int& nextCol,  Vector<coordinateT>& path);
bool wordCanBeFoundOnBoard(Grid<char>& board, string word, Vector<coordinateT>& path, Vector<coordinateT>& alreadyBeen, int curInd = 0);
bool isValidEntry(string word,  Set<string>& enteredWords, Lexicon& english);
void humanPlayerTurn(Grid<char>& board, Set<string>& enteredWords, Lexicon& english);
void generateWords(Grid<char>& board, Set<string>& enteredWords, Lexicon& english, int row, int col, string& word, Vector<coordinateT>& coordinates);
void computersTurn(Grid<char>& board, Set<string>& enteredWords,Lexicon& english);
void generateBoard(Grid<char>& board);
void playGame();
void playAgain();
/* Main program */

int main() {
    GWindow gw(BOGGLE_WINDOW_WIDTH, BOGGLE_WINDOW_HEIGHT);
    initGBoggle(gw);
    welcome();
    giveInstructions();
	playGame();
	playAgain();
    return 0;
}

/* Play Boggle game.  */
void playGame(){
	Grid<char> board(BOARD_SIDE, BOARD_SIDE);
	generateBoard(board);
	Set<string> enteredWords;
	Lexicon english("EnglishWords.dat");
	humanPlayerTurn(board, enteredWords, english);
	computersTurn(board, enteredWords, english);
}

/* Asks user if they want to play again. If yes calls method playGame(). */
void playAgain(){
	while(true){
		string playAgain = getLine("Would you like to play again?  ");
		playAgain = toLowerCase(playAgain);
		if(playAgain == "yes"){
			playGame();
		}else if(playAgain == "no"){
			break;
		}else{
			cout << "Please enter yes or no. " << endl;
		}
	}
}

/* For each char of the board generates all words in english lexicon starting with it.
*/
void computersTurn(Grid<char>& board, Set<string>& enteredWords, Lexicon& english){
	for(int i = 0; i < board.nRows; i ++){
		for(int j = 0; j < board.nCols; j ++){
			string word = string() + board[i][j];
			Vector<coordinateT> coordinates;
			coordinateT c;
			c.row = i;
			c.col = j;
			coordinates.add(c);

			generateWords(board, enteredWords, english, i, j, word, coordinates);
		}		
	}
}

/* If generated word fits the rules, it is displayes. else for each coordinate on the grid we go to every possible direction to form new word.
If english lexicon does not contain such prefix, it is dead-end and we abandon it.
If not call generateWords function recursively to continue generating words using backtracking.
*/
void generateWords(Grid<char>& board, Set<string>& enteredWords, Lexicon& english,
				   int curRow, int curCol, string& word, Vector<coordinateT>& coordinates){
	if(word.size() >= 4 && english.contains(word) && !enteredWords.contains(word)){
		enteredWords.add(word);
		recordWordForPlayer(word, COMPUTER);
		return;
	}

	for(int row = -1; row <= 1; row ++){
		for(int col = -1; col <= 1; col ++){
			if(row == 0 && col == 0){
				continue;
			}
			if(board.inBounds(curRow + row, curCol + col)){
				if(!vectorContains(coordinates, curRow + row, curCol + col)){
					word += board[curRow + row][curCol + col];
					if(english.containsPrefix(word)){
						coordinateT c;
						c.row = curRow + row;
						c.col = curCol + col;
						coordinates.add(c);

						generateWords(board, enteredWords, english, curRow + row, curCol + col, word, coordinates);
						coordinates.remove(coordinates.size()-1);
					}
					word = word.substr(0, word.size()-1);
				}
			}
				
		}
	}
}

/* While user does not input empty string, for each entry we check if it is valid, and then if can be found on the board.
   If so we display it for HUMAN player.
*/
void humanPlayerTurn(Grid<char>& board, Set<string>& enteredWords, Lexicon& english){
	cout << "Ok, take all the time you want and find all the words you can!  Signal that you're finished by entering an empty line" << endl;
	Vector<coordinateT> path;
	while(true){
		string word = getLine("Enter word: ");
		word = toUpperCase(word);

		for(int i = 0; i < path.size(); i ++){
			highlightCube(path[i].row, path[i].col, false);
		}
		path.clear();
		
		if(word == ""){
			return;
		}

		if(!isValidEntry(word, enteredWords, english)){
			continue;
		} 

		bool canBeFound = false;
		
		for(int i = 0; i < board.nRows; i ++){
			for(int j = 0; j < board.nCols; j ++){
				if(board[i][j] == word[0]){
					coordinateT c;
					c.row = i;
					c.col = j;
					path.add(c);

					Vector<coordinateT> alreadyBeen;
					alreadyBeen.add(c);
					if(wordCanBeFoundOnBoard(board, word, path, alreadyBeen)){
						canBeFound = true;
						break;
					}
				}
			}
			if(canBeFound){
				break;
			}
		}

		 if(!canBeFound){
			cout << "You can't make that word!" << endl;
			continue;
		 }  

		enteredWords.add(word);
		recordWordForPlayer(word, HUMAN);	
	}
}

/*	This recursive function checks if given word can be found on the board.
	We check if char on coordinates(curRow,curCol) can be connected to nextChar and if can what are the coordinates of nextChar(need it to highlight cubes in the end.)
*/
bool wordCanBeFoundOnBoard(Grid<char>& board, string word, Vector<coordinateT>& path, Vector<coordinateT>& alreadyBeen, int curInd){
	if(curInd == word.size()-1){
		if(path.isEmpty()){
			return false;
		}
		for(int i = 0; i < path.size(); i ++){
			highlightCube(path[i].row, path[i].col, true);
		}
		return true;
	}

	int curRow = path[path.size()-1].row;
	int curCol = path[path.size()-1].col;
	coordinateT nextCharCoordinate;

	/*	If curChar cant be connected to nextChar does not mean there is not any way.
		On board can be several curChars on different coordinates and we need to check them all.
	*/
	if(!canBeConnected(board, word[curInd+1], curRow, curCol, nextCharCoordinate.row, nextCharCoordinate.col, path)){
		while(true){
			if(curInd == 0 || path.size() < 2){
				path.clear();
				return false;
			}

			curRow = path[path.size()-2].row;
			curCol = path[path.size()-2].col;
			if(canBeConnected(board, word[curInd], curRow, curCol, nextCharCoordinate.row, nextCharCoordinate.col, path)){
				path.remove(path.size()-1);
				if(!vectorContains(alreadyBeen, nextCharCoordinate.row, nextCharCoordinate.col)){
					path.add(nextCharCoordinate);
					curInd --;
				}
				break;
			}
			path.remove(path.size()-1);
			curInd --;
		}
	}else{
		path.add(nextCharCoordinate);
		alreadyBeen.add(nextCharCoordinate);
	}

	return wordCanBeFoundOnBoard(board, word, path, alreadyBeen, curInd+1);
}

/*	check each of 8 possible neighbours to find nextChar. 
*/
bool canBeConnected(Grid<char>& board, char nextChar, int curRow, int curCol, int& nextRow, int& nextCol, Vector<coordinateT>& path){
	for(int row = -1; row <= 1; row ++){
		for(int col = -1; col <= 1; col ++){
			if(row == 0 && col == 0){
				continue;
			}
			if(board.inBounds(curRow + row, curCol + col) && board[curRow + row][curCol + col] == nextChar){
				nextRow = curRow + row;
				nextCol = curCol + col;
				if(!vectorContains(path, nextRow, nextCol)){
					return true;
				}
			}
		}
	}

	return false;
}

/* This method checks if vector of <coordinateT> contains member with coordinates (row,col) */
bool vectorContains(Vector<coordinateT>& path, int row, int col){
	for(int i = 0; i < path.size(); i ++){
		if(path[i].row == row && path[i].col == col){
			return true;
		}
	}

	return false;
}

/*	String is valid entry if it is at least 4 characters long word from english lexicon, that has not been entered yet.
*/
bool isValidEntry(string word, Set<string>& enteredWords, Lexicon& english){
	if(word.size() < 4){
		cout << "Word should be at least 4 symbols long." << endl;
		return false;
	}
	
	if(enteredWords.contains(word)){
		cout << "You have already entered this word." << endl;
		return false;
	}

	if(!english.contains(word)){
		cout << "It isn't english word!" << endl;
		return false;
	}

	return true;
}


void generateBoard(Grid<char>& board){
	Vector<string> cubes;

	bool isUserConfiguration = giveUserChanceToEnterCubeConfigurations(cubes);
	
	arrangeCubesOnBoard(board, cubes, isUserConfiguration);

	drawBoard(BOARD_SIDE, BOARD_SIDE);
	labelCubes(board);
}

/*	If user wants, he/she can enter configurations for the board: 16 strings containing 6 symbols - 16 cubes each with 6 sides.
*/
bool giveUserChanceToEnterCubeConfigurations(Vector<string>& cubes){
	cout << "Do you want to set up board configuration? " << endl;
	string answer = getLine();
	answer = toLowerCase(answer);
	while(true){
		if(answer == "no"){
			return false;
		}else if(answer == "yes"){
			cout << "Enter " << BOARD_SIZE << " string containing 6 symbols" << endl;
			for(int i = 0; i < BOARD_SIZE; i ++){
				string cube = getLine();
				cube = toUpperCase(cube);
				cubes.add(cube);
			}
			return true;
		}else{
			cout << "Please enter yes or no. " << endl;
			answer = getLine();
		}
	}

}

/*	This method arranges cubes on board. If user made chose, we shuffle entered cubes and choose sides.
	If not we transfer given array of string into vector<string> cubes and do the same.
*/
void arrangeCubesOnBoard(Grid<char>& board, Vector<string>& cubes, bool isUserConfiguration){
	if(!isUserConfiguration){
		for(int i = 0; i < BOARD_SIZE; i ++){
			cubes.add(STANDARD_CUBES[i]);
		}
	}

	shuffleCubes(cubes); 
	chooseSideAndFillTheGrid(board, cubes);
}

void shuffleCubes(Vector<string>& cubes){
	for(int i = 0; i < cubes.size(); i ++){
		int r = randomInteger(i, cubes.size()-1);
		swap(cubes[i], cubes[r]);
	}
}

void chooseSideAndFillTheGrid(Grid<char>& board, Vector<string>& cubes){
	for(int i = 0; i < cubes.size(); i ++){
		int side = randomInteger(0,5);
		board[i/BOARD_SIDE][i%BOARD_SIDE] = cubes[i][side];
	}
}

void labelCubes(Grid<char>& board){
	for(int i = 0; i < board.nRows; i ++){
		for(int j = 0; j < board.nCols; j ++){
			labelCube(i, j, board[i][j]);
		}
	}
}
/*
 * Function: welcome
 * Usage: welcome();
 * -----------------
 * Print out a cheery welcome message.
 */

void welcome() {
    cout << "Welcome!  You're about to play an intense game ";
    cout << "of mind-numbing Boggle.  The good news is that ";
    cout << "you might improve your vocabulary a bit.  The ";
    cout << "bad news is that you're probably going to lose ";
    cout << "miserably to this little dictionary-toting hunk ";
    cout << "of silicon.  If only YOU had a gig of RAM..." << endl << endl;
}

/*
 * Function: giveInstructions
 * Usage: giveInstructions();
 * --------------------------
 * Print out the instructions for the user.
 */

void giveInstructions() {
    cout << endl;
    cout << "The boggle board is a grid onto which I ";
    cout << "I will randomly distribute cubes. These ";
    cout << "6-sided cubes have letters rather than ";
    cout << "numbers on the faces, creating a grid of ";
    cout << "letters on which you try to form words. ";
    cout << "You go first, entering all the words you can ";
    cout << "find that are formed by tracing adjoining ";
    cout << "letters. Two letters adjoin if they are next ";
    cout << "to each other horizontally, vertically, or ";
    cout << "diagonally. A letter can only be used once ";
    cout << "in each word. Words must be at least four ";
    cout << "letters long and can be counted only once. ";
    cout << "You score points based on word length: a ";
    cout << "4-letter word is worth 1 point, 5-letters ";
    cout << "earn 2 points, and so on. After your puny ";
    cout << "brain is exhausted, I, the supercomputer, ";
    cout << "will find all the remaining words and double ";
    cout << "or triple your paltry score." << endl << endl;
    cout << "Hit return when you're ready...";
    getLine();
}