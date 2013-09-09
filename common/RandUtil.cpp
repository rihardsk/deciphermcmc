/*
 * RandUtil.cpp
 *
 *  Created on: Jan 28, 2010
 *      Author: cinc
 */

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>
#include "RandUtil.h"
#include "ArrayUtil.h"

using namespace std;

RandUtil::RandUtil() {
	seedRand();
}

int RandUtil::newIntRand(int maxInt) {
	double doubleRand = newRand() * maxInt;
	return (int)doubleRand;
}

double RandUtil::newRand() {
	return drand48();					// unix version
	//return (double(rand()) / RAND_MAX); // windows version
}

void RandUtil::seedRand() {
	cout << "Initialize Random Seed..." << endl;
	struct timeval tmptv;
	gettimeofday (&tmptv, (struct timezone *)NULL);
	int seed = (int) tmptv.tv_usec;
	srand48(seed);	// unix version
	//srand(seed);		// windows version
}

int* RandUtil::shuffle(int size) {
	int* v = (int*) calloc(size, sizeof(int));

	// initialize values
	for (int i=0; i<size; i++) {
		v[i] = i;
	}

	// shuffle
	int pos1, pos2;
	int t;
	int maxIteration = size*2;
	for (int i=0; i<maxIteration; i++) {
		pos1 = newIntRand(size);
		pos2 = newIntRand(size);
		// swap pos1 and pos2
		t = v[pos1];
		v[pos1] = v[pos2];
		v[pos2] = t;
	}
	return v;
}

int* RandUtil::fisherYatesShuffle(int size) {
	int* v = (int*) calloc(size, sizeof(int));

	// initialize values
	for (int i=0; i<size; i++) {
		v[i] = i;
	}

	int k, temp;
	for (int i=size; i>1; i--) {
		k = newIntRand(i);

		// swap v[k] and v[size-1] (the last element)
		temp = v[i-1];
		v[i-1] = v[k];
		v[k] = temp;

		cout << "round " << (size-i) << ", pick " << k << endl;
		ArrayUtil::outputArray("values: ", v, size);

	}

	return v;
}
