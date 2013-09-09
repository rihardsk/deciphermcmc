/*
 * PairSubstitutionCipher.cpp
 *
 * exclude all spaces and other non-alphabetic characters (e.g. ?, {}, 1)
 *
 * keys[26*26]
 *
 *  Created on: Feb 21, 2010
 *      Author: cinc
 */

#include "PairSubstitutionCipher.h"
#include <iostream>
#include <iomanip>
#include <math.h>
#include "../common/RandUtil.h"
#include "../common/ArrayUtil.h"
#include "../common/ConvUtil.h"
#include "../common/SortUtil.h"

using namespace std;

string PairSubstitutionCipher::encrypt(string plaintext) {
	// create key for encryption
	int keylength = NUMLETT*NUMLETT;
	int* key;
	RandUtil* randUtil = new RandUtil();
	key = randUtil->shuffle(keylength);
	ArrayUtil::outputArray("key: ", key, keylength);


	// encryption
	string ciphertext(plaintext);
	char c[2];
	int index1, index2;
	for (int i=0; i<(ciphertext.length()/2); i++) {
		c[0] = ciphertext.at(i*2);
		c[1] = ciphertext.at(i*2+1);
		int index = ConvUtil::pairAlphabetToIntIndex(c);
		//cout << c[0] << c[1] << " " << index;

		ConvUtil::intIndexToPairAlphabet(key[index], c);
		ciphertext[i*2]   = c[0];
		ciphertext[i*2+1] = c[1];
		//cout << " => " << ciphertext.at(i*2) << ciphertext.at(i*2+1) << endl;
	}
	return ciphertext;
}

string PairSubstitutionCipher::decryptUsingPairFrequency(string ciphertext, string trainingtext) {
	RandUtil randUtil;

	int trainingTextFreq[NUMLETT*NUMLETT][NUMLETT*NUMLETT];
	int testTextFreq[NUMLETT*NUMLETT][NUMLETT*NUMLETT];
	countPairFrequency(trainingtext, trainingTextFreq);
	countPairFrequency(ciphertext, testTextFreq);
	ArrayUtil::outputArray("training freq: " , trainingTextFreq[0], NUMLETT*NUMLETT); // output frequencies of [ab][xx]
	ArrayUtil::outputArray("cipher   freq: " , testTextFreq[0], NUMLETT*NUMLETT);     // output frequencies of [ab][xx]


	int f[NUMLETT*NUMLETT], newf[NUMLETT*NUMLETT];
	// initialize decrypting function
	for (int i=0; i<NUMLETT*NUMLETT; i++) {
		f[i] = i;
	}

	double prevlogscore = thelogscoreForPairFrequency(trainingTextFreq, testTextFreq, f);
	double newlogscore;

	int numIteration = 1000;
	int outputInterval = numIteration/5;
	int numAccept = 0;
	double power = 1;

	int pos1, pos2;
	for (int i=0; i<numIteration; i++) {
		// proposal newf by swapping 2 positions of f
		pos1 = randUtil.newIntRand(NUMLETT*NUMLETT);
		pos2 = randUtil.newIntRand(NUMLETT*NUMLETT);
		for (int j=0; j<NUMLETT*NUMLETT; j++) {
			newf[j] = f[j];
		}
		newf[pos1] = f[pos2];
		newf[pos2] = f[pos1];


		newlogscore = thelogscoreForPairFrequency(trainingTextFreq, testTextFreq, newf);

		if (i%outputInterval ==0) {
			cout << "iteration: " << i << " swapping:" << setw(3) << pos1 << "," << setw(3) << pos2 << " prevlogscore: " << prevlogscore << " newlogscore: " << newlogscore;
			cout << " text: " << decrypt(ciphertext.substr(0, 20), f);
		}

		if ( log(randUtil.newRand()) < power * (newlogscore - prevlogscore) ) {
			if (i%outputInterval ==0) cout << " accepted";
		    /* Accept the new function. */
			for (int j=0; j<NUMLETT*NUMLETT; j++) {
				f[j] = newf[j];
			}
		    prevlogscore = newlogscore;
		    numAccept ++;
		} else {
		    /* Reject the new function. */
			if (i%outputInterval ==0) cout << " rejected";
		}
		if (i%outputInterval ==0) cout << endl;

	}

	cout << setprecision(3) << "Acceptance rate: " << (1.0*numAccept)/numIteration << endl;

	ArrayUtil::outputArray("decryption function: ", f, 30);

	// constructing decrypted text:
	return ciphertext; //decrypt(ciphertext, f);


}

string PairSubstitutionCipher::decrypt(string text, int f[NUMLETT*NUMLETT]) {
	char c[2];
	int index;
	for (int i=0; i<(text.length()/2); i++) {
		c[0] = text[i*2];
		c[1] = text[i*2+1];
		index = ConvUtil::pairCharToIntIndex(c);
//		cout << index << "->" << f[index] << endl;

		ConvUtil::intIndexToPairAlphabet(f[index], c);
		text[i*2]  = c[0];
		text[i*2+1] = c[1];
	}
	return text;
}

double PairSubstitutionCipher::thelogscoreForPairFrequency(int traincounts[NUMLETT*NUMLETT][NUMLETT*NUMLETT], int testcounts[NUMLETT*NUMLETT][NUMLETT*NUMLETT], int f[NUMLETT*NUMLETT]) {
    int i,j;
    double tmpscore = 0.0;
    for (i=0; i<NUMLETT*NUMLETT; i++) {
        for (j=0; j<NUMLETT*NUMLETT; j++) {
        	//cout << i << ", " << j << ": " << testcounts[i][j] << ", " << traincounts[f[i]][f[j]] << endl;
        	tmpscore = tmpscore + testcounts[i][j] * log(traincounts[f[i]][f[j]]);
        }
    }
    return(tmpscore * PIPOWER);
}

void PairSubstitutionCipher::countPairFrequency(string text, int frequency[NUMLETT*NUMLETT][NUMLETT*NUMLETT]) {

	// initialize frequency array
	for (int i=0; i<NUMLETT*NUMLETT; i++) {
		for (int j=0; j<NUMLETT*NUMLETT; j++) {
			frequency[i][j] = 1;
		}
	}

	char prevc[2];
	char c[2];

	// read first 2 characters
	prevc[0] = text[0];
	prevc[1] = text[1];

	// read 2 characters each time along the text and count frequencies
	for (int i=1; i<(text.length()/2); i++) {
		c[0] = text[i*2];
		c[1] = text[i*2+1];

//		cout << "[" << prevc[0] << prevc[1] << "] [" << c[0] << c[1] << "], "
//				<< "indxe [" << ConvUtil::pairAlphabetToIntIndex(prevc) << ", " << ConvUtil::pairAlphabetToIntIndex(c) << "] ++" << endl;
		frequency[ ConvUtil::pairAlphabetToIntIndex(prevc) ][ ConvUtil::pairAlphabetToIntIndex(c) ] ++;

		prevc[0] = c[0];
		prevc[1] = c[1];
	}

/*
	// calculate frequencies by sliding moves
	for (int i=0; i<(text.length()-4); i++) {
		prevc[0] = text[i];
		prevc[1] = text[i+1];
		c[0] = text[i+2];
		c[1] = text[i+3];
		frequency[ ConvUtil::pairAlphabetToIntIndex(prevc) ][ ConvUtil::pairAlphabetToIntIndex(c) ] ++;
	}
*/
}


string PairSubstitutionCipher::decryptUsingSingleFrequency(string ciphertext, string trainingtext) {
	int trainingTextSingleFreq[NUMLETT*NUMLETT];
	int cipherTextSingleFreq[NUMLETT*NUMLETT];

	// calculate single letter frequencies for training text and cipher text
	countSinglePairFrequency(trainingtext, trainingTextSingleFreq);
	countSinglePairFrequency(ciphertext,   cipherTextSingleFreq);

	// calculate index order of training text and cipher text
	int trainingTextOrder[NUMLETT*NUMLETT];
	SortUtil::getDescendingOrder(trainingTextSingleFreq, trainingTextOrder, NUMLETT*NUMLETT);
	int cipherTextOrder[NUMLETT*NUMLETT];
	SortUtil::getDescendingOrder(cipherTextSingleFreq, cipherTextOrder, NUMLETT*NUMLETT);

	int f[NUMLETT*NUMLETT];
	for (int i=0; i<NUMLETT*NUMLETT; i++) {
		f[ cipherTextOrder[i] ] = trainingTextOrder[i];
	}

	cout << endl;
	ArrayUtil::outputArray("trainingTextSingleFreq", trainingTextSingleFreq, NUMLETT*NUMLETT);
	ArrayUtil::outputArray("trainingTextOrder", trainingTextOrder, NUMLETT*NUMLETT);
	ArrayUtil::outputArray("cipherTextSingleFreq",   cipherTextSingleFreq,   NUMLETT*NUMLETT);
	ArrayUtil::outputArray("cipherTextOrder",   cipherTextOrder,   NUMLETT*NUMLETT);
	ArrayUtil::outputArray("decypt function f", f, NUMLETT*NUMLETT);

//	char c1[2];
//	ConvUtil::intIndexToPairAlphabet(134, c1);
//	cout << c1[0] << c1[1] << endl;

	// decrypt
	string decryptedText = ciphertext;
	char c[2];
	char dc[2];
	for (int i=0; i<(decryptedText.length()/2); i++) {
		c[0] = ciphertext[i*2];
		c[1] = ciphertext[i*2+1];

		int decryptedCharsIndex = f[ConvUtil::pairAlphabetToIntIndex(c)];
		ConvUtil::intIndexToPairAlphabet(decryptedCharsIndex, dc);

		decryptedText[i*2] = dc[0];
		decryptedText[i*2+1] = dc[1];
	}
	return decryptedText;
}

void PairSubstitutionCipher::countSinglePairFrequency(string text, int frequency[NUMLETT*NUMLETT]) {
	// initialization
	for (int i=0; i<NUMLETT*NUMLETT; i++) {
		frequency[i] = 1;
	}

	char c[2];
	for (int i=0; i<(text.length()/2); i++) {
		c[0] = text[i*2];
		c[1] = text[i*2+1];
		frequency[ ConvUtil::pairAlphabetToIntIndex(c) ] ++;
	}
}
