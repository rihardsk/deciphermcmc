/*
 * SysUtil.h
 *
 *  Created on: Apr 4, 2010
 *      Author: cinc
 */

#ifndef SYSUTIL_H_
#define SYSUTIL_H_

#include <sys/time.h>

using namespace std;

class SysUtil {
public:
	SysUtil();
	virtual ~SysUtil();

	/**
	 * get current time in seconds
	 */
	static double getCurrentTimestamp();


	/**
	 * calculate duraction from input time to current time in seconds
	 */
	static double calculateElapsedTime(double startTimestamp);
};

#endif /* SYSUTIL_H_ */
