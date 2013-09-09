/*
 * ConvUtil.cpp
 *
 *  Created on: Feb 21, 2010
 *      Author: cinc
 */

#include "ConvUtil.h"

#define NUMLETT 26

using namespace std;

int ConvUtil::charToIntIndex(char c) {
    if ( (c >= 'A') && (c <= 'Z') )
        return(c - 'A');
    else if ( (c >= 'a') && (c <= 'z') )
        return(c - 'a');
    else
        return(NUMLETT); /* Non-alphabetic character. */
}

char ConvUtil::intIndexToChar(int index) {
	if ( (index >= 0) && (index < NUMLETT) ) {
		return (char)(index+'A');
	} else {
		return ' ';
	}
}


int ConvUtil::pairCharToIntIndex(char c[2]) {
	return charToIntIndex(c[0]) * (NUMLETT+1) + charToIntIndex(c[1]);
}

void ConvUtil::intIndexToPairChar(int index, char c[2]) {
	int index1 = index / (NUMLETT+1);
	int index2 = index - index1 * (NUMLETT+1);
	c[0] = intIndexToChar( index1 );
	c[1] = intIndexToChar( index2 );
}

int ConvUtil::pairAlphabetToIntIndex(char c[2]) {
	return charToIntIndex(c[0]) * NUMLETT + charToIntIndex(c[1]);
}

void ConvUtil::intIndexToPairAlphabet(int index, char c[2]) {
	int index1 = index / NUMLETT;
	int index2 = index - index1 * NUMLETT;
	c[0] = intIndexToChar( index1 );
	c[1] = intIndexToChar( index2 );
}


string ConvUtil::convertToUpperCase(string input) {
	string output = input;
	int toAdd = 'A'-'a';

	for (int i=0; i<output.length(); i++) {
		if ( (output[i] >= 'a') && (output[i] <='z') ) {
			output[i] += toAdd;
		}
	}
	return output;
}
