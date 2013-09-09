/*
 * SortUtil.cpp
 *
 *  Created on: Feb 18, 2010
 *      Author: cinc
 */

#include "SortUtil.h"

void SortUtil::getDescendingOrder(int values[], int order[], int size) {
	// initialize order and tempvalue array
	int tempvalues[size];
	for (int i=0; i<size; i++) {
		tempvalues[i] = values[i];
		order[i] = i; // position
	}

	// bubble sort, descending
	for (int i=0; i<size-1; i++) {
		for (int j=i+1; j<size; j++) {
			if (tempvalues[i] < tempvalues[j]) {
				// switch value
				int t = tempvalues[i];
				tempvalues[i] = tempvalues[j];
				tempvalues[j] = t;
				// switch position
				t = order[i];
				order[i] = order[j];
				order[j] = t;
			}
		}
	}

}
