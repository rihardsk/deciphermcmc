/*
 * ConvUtil.h
 *
 *  Created on: Feb 21, 2010
 *      Author: cinc
 */

#ifndef CONVUTIL_H_
#define CONVUTIL_H_

#include <string>

using namespace std;

class ConvUtil {
public:
	/**
	 * 'C' -> 2
	 */
	static int charToIntIndex(char c);
	/**
	 * 2 -> 'C'
	 */
	static char intIndexToChar(int index);

	/**
	 * 'BC' -> 1*27+2=29
	 */
	static int pairCharToIntIndex(char c[2]);
	/**
	 * 29=1*27+2 -> 'BC'
	 */
	static void intIndexToPairChar(int index, char c[2]);



	/**
	 * 'BC' -> 1*26+2=28
	 */
	static int pairAlphabetToIntIndex(char c[2]);
	/**
	 * 28=1*26+2 -> 'BC'
	 */
	static void intIndexToPairAlphabet(int index, char c[2]);

	/**
	 * convert input string to upper cases
	 */
	static string convertToUpperCase(string input);
};

#endif /* CONVUTIL_H_ */
