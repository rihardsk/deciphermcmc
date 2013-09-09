/*
 * TextFileUtil.cpp
 *
 *  Created on: Jan 31, 2010
 *      Author: cinc
 */

#include <iostream>
#include <cstdlib>
#include <string>
#include <fstream>
#include "TextFileUtil.h"
#include "ConvUtil.h"

using namespace std;

string TextFileUtil::readTextFromFile(char* filename) {
	ifstream ifs(filename);
	if (ifs.fail()) {
		cout << "File not exist: " << filename << endl;
		exit(1);
	}

	string text, line;
	while( !ifs.eof() ) {
		getline(ifs, line);
		text.append(line);
		//text.append("\n");
	}
	ifs.close();

	return text;
}

void TextFileUtil::writeTextToFile(char* filename, string text) {
	ofstream ofs(filename);
	if (ofs.fail()) {
		cout << "File not exist: " << filename << endl;
		exit(1);
	}
	ofs << text;
	ofs.close();
}

char* TextFileUtil::readBinaryFile(char* filename, long size) {
	ifstream ifs(filename, ios::in|ios::binary);
	char* c = new char[size];
	ifs.read(c, size);
	ifs.close();
	return c;
}

long TextFileUtil::getFileSize(char* filename) {
	ifstream ifs(filename, ios::in|ios::binary|ios::ate);
	ifstream::pos_type size = ifs.tellg();
	return size;
}

string TextFileUtil::getLatinAlphabetString(string str) {
	string outstring = str;

	int newsize = 0;
	for (int i=0; i<str.length(); i++) {
		if (ConvUtil::charToIntIndex(str[i]) != 26) {
			outstring[newsize] = str[i];
			newsize ++;
		}
	}
	return outstring.substr(0, newsize);

	/* extremely slow for long string
	for (int i=outstring.size()-1; i>=0; i--) {
		if (ConvUtil::charToIntIndex(outstring.at(i)) == 26) {
			outstring.erase(i, 1);
		}
	}
	return outstring;
	*/
}
