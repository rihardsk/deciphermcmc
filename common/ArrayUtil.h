/*
 * ArrayUtil.h
 *
 *  Created on: Jan 29, 2010
 *      Author: chenja
 */

#ifndef ARRAYUTIL_H_
#define ARRAYUTIL_H_

#include <string>

using namespace std;

class ArrayUtil {
public:
	/**
	 * create an one-dimension integer array with given size
	 */
	static int*  createIntArray(int size);
	/**
	 * create an two-dimension integer array with given size
	 */
	static int** create2DIntArray(int rowSize, int colSize);

	/**
	 * create an two-dimension double array with given size
	 */
	static double** create2DDoubleArray(int rowSize, int colSize);

	/**
	 * output value of an integer array
	 */
	static void outputArray(string message, int values[], int size);
	/**
	 * output value of an double array
	 */
	static void outputArray(string message, double values[], int size);

	static double average(double values[], int size);

	static double sum(double values[], int size);

	/**
	 * return no of occurrence of a value in the array
	 */
	static int countNoOfOccur(double testvalue, double values[], int size);

	/**
	 * copy values from source array to target array
	 */
	static void copyArray(int source[], int target[], int size);
	/**
	 * copy values from source array to target array
	 */
	static void copyArray(double source[], double target[], int size);

	/**
	 * check if 2 arrays are the same, return true if all elements are the same
	 */
	static bool diff(int values1[], int values2[], int size);
};

#endif /* ARRAYUTIL_H_ */
