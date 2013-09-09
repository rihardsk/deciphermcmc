/*
 * TextFileUtil.h
 *
 *  Created on: Jan 31, 2010
 *      Author: cinc
 */

#ifndef TEXTFILEUTIL_H_
#define TEXTFILEUTIL_H_

#include <string>

using namespace std;

class TextFileUtil {
public:
	static string readTextFromFile(char* filename);
	static void    writeTextToFile(char* filename, string text);
	static char*   readBinaryFile(char* filename, long size);
	static long   getFileSize(char* filename);

	/**
	 * remove spaces, non-alphabetic letters from the string
	 */
	static string getLatinAlphabetString(string str);
};

#endif /* TEXTFILEUTIL_H_ */
