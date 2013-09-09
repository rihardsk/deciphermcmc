/*
 * TranspositionCipher.cpp
 *
 *  Created on: Jan 31, 2010
 *      Author: cinc
 */

#include <iostream>
#include <cstdlib>
#include <iomanip>
#include <string>
#include <math.h>
#include "TranspositionCipher.h"
#include "../common/RandUtil.h"
#include "../common/ConvUtil.h"
#include "../common/ArrayUtil.h"

using namespace std;

string TranspositionCipher::encrypt(string plaintext, string key) {
	int keylength = key.length();


	// key: string -> integer index
	int* intKey = (int*) calloc(keylength, sizeof(int));
	//cout << "key: ";
	for (int i=0; i<keylength; i++) {
		intKey[i] = key.at(i) - '0';
		//cout << intKey[i];
	}
	//cout << endl;

	return encrypt(plaintext, intKey, keylength);
}

string TranspositionCipher::encrypt(string plaintext, int key[], int keylength) {
	// output encryption key.
	ArrayUtil::outputArray("Encryption key: ", key, keylength);

	string ciphertext (plaintext.length(), ' ');
	// encrypt
	int iteration = (int)(plaintext.length()/keylength);
	for (int i=0; i<iteration; i++) {
		for (int j=0; j<keylength; j++) {
			ciphertext[i*keylength+j]= plaintext.at(i*keylength+key[j]);
		}
		//cout << endl;
	}

	return ciphertext;
}

void TranspositionCipher::swapKey(int testKey[], int testNewKey[], int keylength, int pos1, int pos2) {
    // swap key
    for(int j = 0;j < keylength;j++){
        testNewKey[j] = testKey[j];
    }
    testNewKey[pos2] = testKey[pos1];
    testNewKey[pos1] = testKey[pos2];
}

void TranspositionCipher::slideMove(int* key, int* newKey, int keylength, int pos1, int pos2) {
//    cout << "key: ";
//    for(int i=0; i<keylength; i++) { cout << key[i]; }
//    cout << " swap(" << pos1 << "," << pos2 << ")";
//    cout << " newkey: ";

    for (int i=0; i<keylength; i++) {
    	newKey[i] = key[i];
    }
    if (pos1 < pos2) {
    	// slide keys[pos1..pos2] left
    	for (int i=pos1; i<pos2; i++) {
    		newKey[i] = key[i+1];
    	}
    	newKey[pos2] = key[pos1];
    } else if (pos2 < pos1) {
    	// slide keys[pos1..pos2] right
    	for (int i=pos2; i<pos1; i++) {
    		newKey[i+1] = key[i];
    	}
    	newKey[pos2] = key[pos1];
    }

//    for(int i=0; i<keylength; i++) { cout << newKey[i]; }
//    cout << endl;
}

void TranspositionCipher::blockSlideMove(int *key, int* newKey, int keylength,  int pos1, int pos2, int blocksize) {
    for (int i=0; i<keylength; i++) {
    	newKey[i] = key[i];
    }
    if ( (pos1+blocksize-1) < pos2) {
    	for (int i=pos1; i<(pos2-blocksize+1); i++) {
    		newKey[i] = key[i+blocksize];
    	}
    	for (int i=0; i<blocksize; i++) {
        	newKey[pos2-blocksize+i+1] = key[pos1+i];
    	}
    }
    if ( (pos2<pos1) && ((pos1+blocksize-1)<keylength) ) {
    	for (int i=pos2; i<pos1; i++) {
    		newKey[i+blocksize] = key[i];
    	}
    	for (int i=0; i<blocksize; i++) {
    		newKey[pos2+i] = key[pos1+i];
    	}
    }
}

string TranspositionCipher::decryptUsingPairFrequency(string ciphertext, string trainingtext, int keylength, int numIteration) {
	int trainingTextPairFreq[27][27];
	int testTextPairFreq[27][27];
	int testNewTextPairFreq[27][27];
	string testPlaintext = ciphertext;
	string testNewPlaintext = ciphertext;
	RandUtil* randUtil = new RandUtil();

	countPairFrequency(trainingtext, trainingTextPairFreq);
	countPairFrequency(testPlaintext, testTextPairFreq);

	// initialize decrypt key = 123456...
	int* testKey = (int*) calloc(keylength, sizeof(int));
	int* testNewKey = (int*) calloc(keylength, sizeof(int));
	for (int i=0; i<keylength; i++) {
		testKey[i] = i;
		testNewKey[i] = i;
	}

	int outputInterval = numIteration/5;
	int numAccept = 0;
	int pos1=0, pos2=0;

	double power = 1;

	// whether to remember the max log score
	bool rememberMaxLogscore = true;
	double maxLogscore = 0;
	int* keyMaxLogscore = (int*) calloc(keylength, sizeof(int));
	string plaintextMaxLogscore;

	//double power = .005;    // power for 15-byte key with slide move
	//double power = .00005;  // correct power for 8-byte key with slide move
	// double power = .00005; // correct power for 5-byte key with swapping keys

	double logscore = thelogscore(trainingTextPairFreq, testTextPairFreq);
	double newlogscore;

	cout << "power: " << setprecision(8) << power << endl;

	for (int i=0; i<numIteration; i++) {
		// swap move
//		pos1 = randUtil->newIntRand(keylength);
//		pos2 = randUtil->newIntRand(keylength);
//		swapKey(testKey, testNewKey, keylength, pos1, pos2);

		// slide move of single letters
//		pos1 = randUtil->newIntRand(keylength);
//		pos2 = randUtil->newIntRand(keylength);
//		slideMove(testKey, testNewKey, keylength, pos1, pos2);

		// slide move of block of letters
		int blocksize = randUtil->newIntRand(keylength-2) + 1; //randUtil->newIntRand(keylength-2) + 1;
		pos1 = randUtil->newIntRand(keylength-blocksize+1);	// fixed a bug
		pos2 = randUtil->newIntRand(keylength-blocksize+1);
		blockSlideMove(testKey, testNewKey, keylength, pos1, pos2, blocksize);

		// swap pos1 and pos2 in testNewPlaintext
		for(int j = 0;j < ciphertext.length();j++){
			int j_index = j%keylength;
			testNewPlaintext[j] = ciphertext[(j-j_index) + testNewKey[j_index]];
		}
		countPairFrequency(testNewPlaintext, testNewTextPairFreq);
		newlogscore = thelogscore(trainingTextPairFreq, testNewTextPairFreq);

		if (i%outputInterval ==0) {
			cout << "Iteration " << setw(4) << i << ": ";
			cout << "block " << blocksize << " ";
			cout << "slide move " << setw(2) << pos1 << " " << setw(2) << pos2 << ". ";
			cout << "text: " << testPlaintext.substr(0, 20) << " newtext: " << testNewPlaintext.substr(0, 20) << ". ";
			cout << "logscore " << fixed << setprecision(0) << logscore << " newlogscore " << newlogscore << ". ";
		}

		// consider whether to accept the proposal
		if (log(randUtil->newRand()) < power*(newlogscore - logscore) ) {
			if (i%outputInterval ==0) cout << "accept. ";
			numAccept++;
			// accept
			ArrayUtil::copyArray(testNewKey, testKey, keylength);
			testPlaintext = testNewPlaintext;
			logscore = newlogscore;

			if (maxLogscore < logscore) {
				maxLogscore = logscore;
				// remember current test key
				ArrayUtil::copyArray(testKey, keyMaxLogscore, keylength);
				plaintextMaxLogscore = testPlaintext;
			}

		} else {
			// reject
			if (i%outputInterval ==0) cout << "reject. ";
		}
		if (i%outputInterval ==0) {
			cout << " key: ";
			for (int j=0; j<keylength; j++) {
				cout << testKey[j];
			}
			cout << " " << endl;
		}
	}
	cout << setprecision(3) << "Acceptance rate: " << (1.0*numAccept)/numIteration << endl;

	if (rememberMaxLogscore) {
		if (logscore < maxLogscore) {
			cout << "last key is different with key with max log score!!" << endl;
			ArrayUtil::outputArray("last key: ", testKey, keylength);
			ArrayUtil::outputArray("max  key: ", keyMaxLogscore, keylength);

			ArrayUtil::copyArray(keyMaxLogscore, testKey, keylength);
			testPlaintext = plaintextMaxLogscore;
		}
	}

	lastresult.keylength = keylength;
	lastresult.decryptionkey = testKey;
	lastresult.logscore = logscore;

	ArrayUtil::outputArray("Decryption key: ", testKey, keylength);
	// return decrypted text
	return testPlaintext;
}

string TranspositionCipher::decryptByKey(string ciphertext, int key[], int keylength) {
	string decryptedtext (ciphertext.length(), ' ');

	int iteration = (int)(ciphertext.length()/keylength);
	for (int i=0; i<iteration; i++) {
		for (int j=0; j<keylength; j++) {
			decryptedtext[i*keylength+j]= ciphertext.at(i*keylength+key[j]);
		}
		//cout << endl;
	}
	return decryptedtext;
}

string TranspositionCipher::decryptByKey(string ciphertext, string key) {
	int keylength = key.length();

	// key: string -> integer index
	int* intKey = (int*) calloc(keylength, sizeof(int));
	//cout << "key: ";
	for (int i=0; i<keylength; i++) {
		intKey[i] = key.at(i) - '0';
		//cout << intKey[i];
	}
	//cout << endl;

	return decryptByKey(ciphertext, intKey, keylength);
}

double TranspositionCipher::thelogscore(int trainingTextPairFreq[27][27], int testTextPairFreq[27][27]) {
	double tempscore = 0.0;
	for(int i=0; i<27; i++) {
		for (int j=0; j<27; j++) {
			tempscore += testTextPairFreq[i][j] * log( trainingTextPairFreq[i][j] );
		}
	}
	return tempscore/4;
}

void TranspositionCipher::countPairFrequency(string text, int pairFreqency[27][27]) {

	// initialize array
	for(int i=0; i<27; i++) {
		for (int j=0; j<27; j++) {
			pairFreqency[i][j] = 1;
		}
	}

	int prevnum = ConvUtil::charToIntIndex(text.at(0));
	int newnum;
	// calculate pair frequency
	for (int i=1; i<text.length(); i++) {
		newnum = ConvUtil::charToIntIndex(text.at(i));
		//cout << "pairFreq[" << prevnum << "," << newnum << "]++" << endl;
		pairFreqency[prevnum][newnum] ++;
		prevnum = newnum;
	}
	return;
}

double TranspositionCipher::verify(string plaintext, string decrypttext, int period) {
	bool isDebug = false;
	if (isDebug) cout << "-> TranspositionCipher::verify()" << endl;

	plaintext = ConvUtil::convertToUpperCase(plaintext).substr(0, period);
	decrypttext = ConvUtil::convertToUpperCase(decrypttext).substr(0, period);
	if (isDebug) {
		cout << "plain     text: " << plaintext << endl;
		cout << "decrypted text: " << decrypttext << endl;
	}
	int noCorrectPositions = 0;

	// first 2 letters
//	if (plaintext.substr(0, 2) == decrypttext.substr(0, 2)) {
//		if (isDebug) cout << "letters in start position correct." << endl;
//		noCorrectPositions ++;
//	}
	// letters in the middle
	for (int i=0; i<period-2; i++) {
		string totest = decrypttext.substr(i, 3);
		if (isDebug) cout << "totest: " << totest;
		if (plaintext.find(totest, 0) != string::npos ) {
			noCorrectPositions++;
			if (isDebug) cout << ", position correct. " << plaintext.find(totest, 0);
		}
		if (isDebug) cout << endl;
	}
	// last 2 letters
//	if (plaintext.substr(period-2, 2) == decrypttext.substr(period-2, 2)) {
//		if (isDebug) cout << "letters in end position correct." << endl;
//		noCorrectPositions ++;
//	}
	double correctness = 1.0 * noCorrectPositions/(period-2);
	cout << "Correct positions: " << noCorrectPositions << " out of " << (period-2) << ". Percentage: " << setprecision(4) << correctness << endl;

	if (isDebug) cout << "<- TranspositionCipher::verify()" << endl;
	return correctness;
}

string TranspositionCipher::decrypt_FinalVersion(string ciphertext, string trainingtext, int ciphertextlength, int keylength, int numIteration) {
	int trainingTextPairFreq[27][27];
	int decryptedTextPairFreq[27][27];
	int numRepetition = 5;

	RandUtil* randUtil = new RandUtil();

	double bestlogscore = 0;
	string bestdecryptedtext;

	for (int i=0; i<numRepetition; i++) {
		cout << endl << "Repetition " << i+1 << "/" << numRepetition << endl;

		int randomposition = randUtil->newIntRand(ciphertext.length()-2000) / keylength * keylength;
		string randomciphertext = ciphertext.substr(randomposition, 2000);
//		cout << "random position: " << randomposition << endl;
//		cout << "random cipher text: " << randomciphertext << endl;
		string decryptedtext = decryptUsingPairFrequency(randomciphertext, trainingtext, keylength, numIteration);

		decryptedtext = decryptByKey(ciphertext, lastresult.decryptionkey, keylength);
		cout << "decrypted full text: " << decryptedtext.substr(0, 80) << endl;
		countPairFrequency(trainingtext, trainingTextPairFreq);
		countPairFrequency(decryptedtext, decryptedTextPairFreq);

		double logscore = thelogscore(trainingTextPairFreq, decryptedTextPairFreq);
		cout << "log score for full text: " << logscore << endl;
		if (bestlogscore < logscore) {
			bestlogscore = logscore;
			bestdecryptedtext = decryptedtext;
		}
	}
	cout << endl << "best log score: " << bestlogscore << endl;
	return bestdecryptedtext;
}

string TranspositionCipher::decryptWithUnknownKeyLength(string ciphertext, string trainingtext, int ciphertextlength, int minkeylength, int maxkeylength) {
	int trainingTextPairFreq[27][27];
	int decryptedTextPairFreq[27][27];

	double bestlogscore = 0;
	string bestdecryptedtext = "     ";
	int bestkeylength = 0;

	for (int keylength=minkeylength; keylength<maxkeylength; keylength++) {
		cout << endl << "try key length: " << keylength << endl;

		int numIteration = 0;
		if (keylength>20) {
			numIteration = 50*1000;
		} else if (keylength>10) {
			numIteration = 10*1000;
		} else {
			numIteration = 2*1000;
		}
		cout << "num of iteration: " << numIteration << endl;
		string decryptedtext = decrypt_FinalVersion(ciphertext, trainingtext, ciphertextlength, keylength, numIteration);

		decryptedtext = decryptByKey(ciphertext, lastresult.decryptionkey, keylength);
		cout << "decrypted full text: " << decryptedtext.substr(0, 80) << endl;
		countPairFrequency(trainingtext, trainingTextPairFreq);
		countPairFrequency(decryptedtext, decryptedTextPairFreq);

		double logscore = thelogscore(trainingTextPairFreq, decryptedTextPairFreq);
		cout << "log score for full text: " << logscore << endl;
		if (bestlogscore < logscore) {
			bestlogscore = logscore;
			bestdecryptedtext = decryptedtext;
			bestkeylength = keylength;
		}
	}
	cout << endl << "best log score: " << bestlogscore << endl;
	cout << "best key length: " << bestkeylength << endl;
	return bestdecryptedtext;
}
