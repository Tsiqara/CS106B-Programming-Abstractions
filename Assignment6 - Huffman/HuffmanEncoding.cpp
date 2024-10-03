/**********************************************************
 * File: HuffmanEncoding.cpp
 *
 * Implementation of the functions from HuffmanEncoding.h.
 * Most (if not all) of the code that you write for this
 * assignment will go into this file.
 */

#include "HuffmanEncoding.h"


/* Function: getFrequencyTable
 * Usage: Map<ext_char, int> freq = getFrequencyTable(file);
 * --------------------------------------------------------
 * Given an input stream containing text, calculates the
 * frequencies of each character within that text and stores
 * the result as a Map from ext_chars to the number of times
 * that the character appears.
 *
 * This function will also set the frequency of the PSEUDO_EOF
 * character to be 1, which ensures that any future encoding
 * tree built from these frequencies will have an encoding for
 * the PSEUDO_EOF character.
 */
Map<ext_char, int> getFrequencyTable(istream& file) {
	Map<ext_char, int> frequencies;
	char ch;
	while(file.get(ch)){
		if(frequencies.containsKey(ch)){
			frequencies[ch] ++;
		}else {
			frequencies.put(ch, 1);
		}
	}
	frequencies.put(PSEUDO_EOF, 1);

	return frequencies;	
}

/* Function: fillUpPriorityQueue
 * Usage: fillUpPriorityQueue(frequencies, pqueue);
 * --------------------------------------------------------
 * given map of frequencies creates priority queue of Nodes
 */
void fillUpPriorityQueue(Map<ext_char, int>& frequencies, PriorityQueue<Node*>& data){
	foreach(ext_char symbol in frequencies){
		Node* curNode = new Node;
		curNode->character = symbol;
		curNode->weight = frequencies.get(symbol);
		curNode->one = NULL;
		curNode->zero = NULL;
		data.enqueue(curNode, (double)curNode->weight);
	}
}

/* Recursive function for buildEncodingTree */
void buildEncodingTreeRec(PriorityQueue<Node*>& data, Map<ext_char, int>& frequencies, Node* & root){
	if(data.size() == 1){
		root = data.dequeue();
		if(root->one != NULL && root->zero != NULL){
			root->weight = root->one->weight + root->zero->weight;
		}
		return;
	}

	Node* leftChild = data.dequeue();
	Node* rightChild = data.dequeue();

	Node* curParent = new Node;
	curParent->zero = leftChild;
	curParent->one = rightChild;
	curParent->character = NOT_A_CHAR;
	curParent->weight = leftChild->weight + rightChild->weight;

	data.enqueue(curParent, curParent->weight);
	buildEncodingTreeRec(data, frequencies, root);
}

/* Function: buildEncodingTree
 * Usage: Node* tree = buildEncodingTree(frequency);
 * --------------------------------------------------------
 * Given a map from extended characters to frequencies,
 * constructs a Huffman encoding tree from those frequencies
 * and returns a pointer to the root.
 *
 * This function can assume that there is always at least one
 * entry in the map, since the PSEUDO_EOF character will always
 * be present.
 */
Node* buildEncodingTree(Map<ext_char, int>& frequencies) {
	PriorityQueue<Node*> data;
	fillUpPriorityQueue(frequencies, data);
	Node* root;
	buildEncodingTreeRec(data, frequencies, root);

	return root;
}

/* Function: freeTree
 * Usage: freeTree(encodingTree);
 * --------------------------------------------------------
 * Deallocates all memory allocated for a given encoding
 * tree.
 */
void freeTree(Node* root) {
	if(root == NULL){
		return;
	}

	freeTree(root->one);
	freeTree(root->zero);
	delete root;
}

/* get code for given ext_char from huffman tree. */
bool getCode(ext_char ch, Node* encodingTree, string& code){
	if(encodingTree == NULL){
		return false;
	}

	if(encodingTree->character == ch){
		return true;
	}

	if(getCode(ch, encodingTree->one, code)){
		code = "1" + code;
		return true;
	}else if (getCode(ch, encodingTree->zero, code)){
		code = "0" + code;
		return true;
	}
}

/* Function: encodeFile
 * Usage: encodeFile(source, encodingTree, output);
 * --------------------------------------------------------
 * Encodes the given file using the encoding specified by the
 * given encoding tree, then writes the result one bit at a
 * time to the specified output file.
 *
 * This function can assume the following:
 *
 *   - The encoding tree was constructed from the given file,
 *     so every character appears somewhere in the encoding
 *     tree.
 *
 *   - The output file already has the encoding table written
 *     to it, and the file cursor is at the end of the file.
 *     This means that you should just start writing the bits
 *     without seeking the file anywhere.
 */ 
void encodeFile(istream& infile, Node* encodingTree, obstream& outfile) {
	//encode all characters in file
	char ch;
	while(infile.get(ch)){
		string code = "";
		getCode(ch, encodingTree, code);
		for(int i = 0; i < code.size(); i ++){
			outfile.writeBit(code[i] - '0');
		}
	}

	//encode PSEUDO_EOF
	string code = "";
	getCode(PSEUDO_EOF, encodingTree, code);
	for(int i = 0; i < code.size(); i ++){
		outfile.writeBit(code[i] - '0');
	}
}

/* Function: decodeFile
 * Usage: decodeFile(encodedFile, encodingTree, resultFile);
 * --------------------------------------------------------
 * Decodes a file that has previously been encoded using the
 * encodeFile function.  You can assume the following:
 *
 *   - The encoding table has already been read from the input
 *     file, and the encoding tree parameter was constructed from
 *     this encoding table.
 *
 *   - The output file is open and ready for writing.
 */
void decodeFile(ibstream& infile, Node* encodingTree, ostream& file) {
	Node* root = encodingTree;
	while(true){
		int bit = infile.readBit();
		if(bit == -1){
			return;
		}
		if(bit == 1 && encodingTree->one != NULL){
			encodingTree = encodingTree->one;
		}else if(bit == 0 && encodingTree->zero != NULL){
			encodingTree = encodingTree->zero;
		}

		if(encodingTree->one == NULL && encodingTree->zero == NULL){
			if(encodingTree->character == PSEUDO_EOF){
				break;
			}
			file.put(encodingTree->character);
			encodingTree = root;
		}
	} 
}

/* Function: writeFileHeader
 * Usage: writeFileHeader(output, frequencies);
 * --------------------------------------------------------
 * Writes a table to the front of the specified output file
 * that contains information about the frequencies of all of
 * the letters in the input text.  This information can then
 * be used to decompress input files once they've been
 * compressed.
 *
 * This function is provided for you.  You are free to modify
 * it if you see fit, but if you do you must also update the
 * readFileHeader function defined below this one so that it
 * can properly read the data back.
 */
void writeFileHeader(obstream& outfile, Map<ext_char, int>& frequencies) {
	/* The format we will use is the following:
	 *
	 * First number: Total number of characters whose frequency is being
	 *               encoded.
	 * An appropriate number of pairs of the form [char][frequency][space],
	 * encoding the number of occurrences.
	 *
	 * No information about PSEUDO_EOF is written, since the frequency is
	 * always 1.
	 */
	 
	/* Verify that we have PSEUDO_EOF somewhere in this mapping. */
	if (!frequencies.containsKey(PSEUDO_EOF)) {
		error("No PSEUDO_EOF defined.");
	}
	
	/* Write how many encodings we're going to have.  Note the space after
	 * this number to ensure that we can read it back correctly.
	 */
	outfile << frequencies.size() - 1 << ' ';
	
	/* Now, write the letter/frequency pairs. */
	foreach (ext_char ch in frequencies) {
		/* Skip PSEUDO_EOF if we see it. */
		if (ch == PSEUDO_EOF) continue;
		
		/* Write out the letter and its frequency. */
		outfile << char(ch) << frequencies[ch] << ' ';
	}
}

/* Function: readFileHeader
 * Usage: Map<ext_char, int> freq = writeFileHeader(input);
 * --------------------------------------------------------
 * Reads a table to the front of the specified input file
 * that contains information about the frequencies of all of
 * the letters in the input text.  This information can then
 * be used to reconstruct the encoding tree for that file.
 *
 * This function is provided for you.  You are free to modify
 * it if you see fit, but if you do you must also update the
 * writeFileHeader function defined before this one so that it
 * can properly write the data.
 */
Map<ext_char, int> readFileHeader(ibstream& infile) {
	/* This function inverts the mapping we wrote out in the
	 * writeFileHeader function before.  If you make any
	 * changes to that function, be sure to change this one
	 * too!
	 */
	Map<ext_char, int> result;
	
	/* Read how many values we're going to read in. */
	int numValues;
	infile >> numValues;
	
	/* Skip trailing whitespace. */
	infile.get();
	
	/* Read those values in. */
	for (int i = 0; i < numValues; i++) {
		/* Get the character we're going to read. */
		ext_char ch = infile.get();
		
		/* Get the frequency. */
		int frequency;
		infile >> frequency;
		
		/* Skip the space character. */
		infile.get();
		
		/* Add this to the encoding table. */
		result[ch] = frequency;
	}
	
	/* Add in 1 for PSEUDO_EOF. */
	result[PSEUDO_EOF] = 1;
	return result;
}

/* Function: compress
 * Usage: compress(infile, outfile);
 * --------------------------------------------------------
 * Main entry point for the Huffman compressor.  Compresses
 * the file whose contents are specified by the input
 * ibstream, then writes the result to outfile.  Your final
 * task in this assignment will be to combine all of the
 * previous functions together to implement this function,
 * which should not require much logic of its own and should
 * primarily be glue code.
 */
void compress(ibstream& infile, obstream& outfile) {
	Map<ext_char, int> freq = getFrequencyTable(infile);
	Node* tree = buildEncodingTree(freq);
	writeFileHeader(outfile, freq);
	infile.rewind();
	encodeFile(infile, tree, outfile);
	freeTree(tree);
}

/* Function: decompress
 * Usage: decompress(infile, outfile);
 * --------------------------------------------------------
 * Main entry point for the Huffman decompressor.
 * Decompresses the file whose contents are specified by the
 * input ibstream, then writes the decompressed version of
 * the file to the stream specified by outfile.  Your final
 * task in this assignment will be to combine all of the
 * previous functions together to implement this function,
 * which should not require much logic of its own and should
 * primarily be glue code.
 */
void decompress(ibstream& infile, ostream& outfile) {
	Map<ext_char, int> freq = readFileHeader(infile);
	Node* tree = buildEncodingTree(freq);
	decodeFile(infile, tree, outfile);
	freeTree(tree);
}

