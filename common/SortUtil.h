/*
 * SortUtil.h
 *
 *  Created on: Feb 18, 2010
 *      Author: cinc
 */

#ifndef SORTUTIL_H_
#define SORTUTIL_H_

class SortUtil {
public:
	/**
	 * return descending order of the index of an array
	 *
	 * e.g. values[10, 1, 3, 15] will give order[3, 0, 2, 1]
	 *      i.e. the index of the largest  number is 3
	 *           the index of the smallest number is 1
	 */
	static void getDescendingOrder(int values[], int order[], int size);
};

#endif /* SORTUTIL_H_ */
