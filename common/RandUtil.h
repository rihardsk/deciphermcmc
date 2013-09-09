/*
 * RandUtil.h
 *
 *  Created on: Jan 28, 2010
 *      Author: cinc
 */

#ifndef RANDUTIL_H_
#define RANDUTIL_H_

class RandUtil {
private:
	/**
	 * Initialize the random seed
	 */
	void seedRand();
public:
	/**
	 * Constructor
	 */
	RandUtil();
	/**
	 * return a random integer between 0..maxInt
	 */
	int newIntRand(int maxInt);

	/**
	 * return a random number between 0 and 1
	 */
	double newRand();

	/**
	 * shuffle, return the random order of an int array start from 0
	 *
	 * e.g. size = 5
	 * return: 31024
	 */
	int* shuffle(int size);

	/**
	 * Fisher–Yates_shuffle (Durstenfeld's in-place shuffle)
	 *
	 */
	int* fisherYatesShuffle(int size);
};

#endif /* RANDUTIL_H_ */
