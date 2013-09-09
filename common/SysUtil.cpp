/*
 * SysUtil.cpp
 *
 *  Created on: Apr 4, 2010
 *      Author: cinc
 */

#include "SysUtil.h"

SysUtil::SysUtil() {
	// TODO Auto-generated constructor stub

}

SysUtil::~SysUtil() {
	// TODO Auto-generated destructor stub
}

double SysUtil::getCurrentTimestamp() {
	struct timeval tv;
	gettimeofday (&tv, (struct timezone *)0);
	return tv.tv_sec + tv.tv_usec/1000000.0;
}

double SysUtil::calculateElapsedTime(double startTimestamp) {
	double currentTimestamp = getCurrentTimestamp();
	return currentTimestamp - startTimestamp;
}
