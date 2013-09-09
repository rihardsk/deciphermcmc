/*
 * ArrayUtil.cpp
 *
 *  Created on: Nov 20, 2009
 *      Author: chenja
 */

#include "ArrayUtil.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <stdlib.h>
#include <sys/time.h>

using namespace std;

int* ArrayUtil::createIntArray(int size) {
	int* a = (int*) calloc(size, sizeof(int));
	return a;
}

int** ArrayUtil::create2DIntArray(int rowSize, int colSize) {
	int** a = (int**)calloc(rowSize, sizeof(*a));
	for (int row=0; row<rowSize; row++) {
		a[row] = (int*)calloc(colSize, sizeof(int));
	}
	return a;
}

double** ArrayUtil::create2DDoubleArray(int rowSize, int colSize) {
	double** a = (double**)calloc(rowSize, sizeof(*a));
	for (int row=0; row<rowSize; row++) {
		a[row] = (double*)calloc(colSize, sizeof(int));
	}
	return a;
}

void ArrayUtil::outputArray(string message, int values[], int size) {
	cout << message << " { ";
	for (int i=0; i<size; i++) {
		cout << i << "=" << values[i] << " ";
	}
	cout << "}" << endl;
}

void ArrayUtil::outputArray(string message, double values[], int size) {
	cout << message << " { ";
	for (int i=0; i<size; i++) {
		cout << i << "=" << setprecision(4) << values[i] << " ";
	}
	cout << "}" << endl;
}

double ArrayUtil::average(double values[], int size) {
	return sum(values, size) / size;
}


double ArrayUtil::sum(double values[], int size) {
	double sum = 0;
	for (int i=0; i<size; i++) {
		sum += values[i];
	}
	return sum;
}

int ArrayUtil::countNoOfOccur(double testvalue, double values[], int size) {
	int count = 0;
	for (int i=0; i<size; i++) {
		if (testvalue == values[i]) {
			count ++;
		}
	}
	return count;
}

void ArrayUtil::copyArray(int source[], int target[], int size) {
	for (int i=0; i<size; i++) {
		target[i] = source[i];
	}
}

void ArrayUtil::copyArray(double source[], double target[], int size) {
	for (int i=0; i<size; i++) {
		target[i] = source[i];
	}
}

bool ArrayUtil::diff(int values1[], int values2[], int size) {
	for (int i=0; i<size; i++) {
		if (values1[i] != values2[i]) {
			return true;
		}
	}
	return false;
}
