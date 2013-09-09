/*
 * SubstitutionCipher.cpp
 *
 *  Created on: Jan 28, 2010
 *      Author: cinc
 */

#include "SubstitutionCipher.h"

#include <iostream>
#include <cstdlib>
#include <iomanip>
#include <math.h>
#include <string>
#include "../common/RandUtil.h"
#include "../common/SortUtil.h"
#include "../common/ArrayUtil.h"
#include "../common/ConvUtil.h"

using namespace std;

string SubstitutionCipher::encrypt(string plaintext) {
	RandUtil randUtil;

	// create a key file
	int* f = randUtil.shuffle(26);
	cout << "Encryption function: ";
	for (int i=0; i<NUMLETT; i++) {
		cout << ConvUtil::intIndexToChar(f[i]);
	}
	cout << endl;

	string ciphertext = plaintext;
	for (int i=0; i<ciphertext.length(); i++) {
		if (ciphertext[i] != ' ') {
			int index = ConvUtil::charToIntIndex( plaintext[i] );
			ciphertext[i] = ConvUtil::intIndexToChar( f[index] );
		}
	}

	return ciphertext;
}

string SubstitutionCipher::decryptByKey(string ciphertext, int f[NUMLETT + 1])
{
	string decryptedText = ciphertext;
    // constructing decrypted text:
    for(int i = 0;i < ciphertext.length();i++){
        if(ciphertext[i] != ' '){
            decryptedText[i] = 'A' + f[ConvUtil::charToIntIndex(ciphertext[i])];
        }
    }

    return decryptedText;
}

string SubstitutionCipher::decryptUsingPairFrequency(string ciphertext, string trainingtext, int numIteration) {
	string decryptedText = ciphertext;
	int trainingTextPairFreq[NUMLETT+1][NUMLETT+1];
	int testTextPairFreq[NUMLETT+1][NUMLETT+1];
	int* testkey  = (int*) calloc(NUMLETT+1, sizeof(int));
	int* testnewkey = (int*) calloc(NUMLETT+1, sizeof(int));
	RandUtil randUtil;
	int pos1, pos2;
	countPairFrequency(trainingtext, trainingTextPairFreq);
	countPairFrequency(ciphertext, testTextPairFreq);

    /* Initialize decoding function f. */
    for (int i=0; i<(NUMLETT+1); i++) {
    	testkey[i] = i;
    }
    double prevlogscore = thelogscoreForPairFrequency(trainingTextPairFreq, testTextPairFreq, testkey);

//	int numIteration = 10*1000;
	int outputInterval = numIteration/5;
	int numAccept = 0;
	double power = 1;

	bool rememberMaxLogScore = true;
	double maxLogScore = 0;
	int* keyMaxLogscore = (int*) calloc(NUMLETT+1, sizeof(int));

	cout << "No. Iterations: " << numIteration << endl;
	cout << "Power: " << power << endl;

	for (int i=0; i<numIteration; i++) {
		// propose newf by swapping 2 positions of f
		pos1 = randUtil.newIntRand(NUMLETT+1);
		pos2 = randUtil.newIntRand(NUMLETT+1);
		ArrayUtil::copyArray(testkey, testnewkey, NUMLETT+1);
		testnewkey[pos1] = testkey[pos2];
		testnewkey[pos2] = testkey[pos1];

		double newlogscore = thelogscoreForPairFrequency(trainingTextPairFreq, testTextPairFreq, testnewkey);
		if (i%outputInterval == 0) {
			cout << "iteration: " << setw(4) << i << " swapping:" << setw(2) << pos1 << "," << setw(2) << pos2 << " prevlogscore: " << fixed << setprecision(0) << prevlogscore << " newlogscore: " << newlogscore;
			cout << " text: " << decryptByKey(ciphertext.substr(0, 40), testkey) << ".";
		}

		if ( log(randUtil.newRand()) < power*(newlogscore - prevlogscore) ) {
			if (i%outputInterval == 0) cout << " accepted";
		    /* Accept the new function. */
			ArrayUtil::copyArray(testnewkey, testkey, NUMLETT+1);
//			for (int j=0; j<=(NUMLETT+1); j++) {
//				f[j] = newf[j];
//			}
		    prevlogscore = newlogscore;
		    numAccept ++;

		    if (rememberMaxLogScore) {
			    if (maxLogScore < newlogscore) {
			    	maxLogScore = newlogscore;
			    	ArrayUtil::copyArray(testkey, keyMaxLogscore, NUMLETT+1);
			    }
		    }
		} else {
		    /* Reject the new function. */
			if (i%outputInterval == 0) cout << " rejected";
		}
		if (i%outputInterval == 0) cout << endl;
	}

	cout << setprecision(3) << "Acceptance rate: " << (1.0*numAccept)/numIteration << endl;

	if (rememberMaxLogScore) {
//		if (ArrayUtil::diff(fMaxLogscore, f, NUMLETT+1)) {
//			cout << "last key is different with key with max log score!!" << endl;
//			ArrayUtil::outputArray("last key: ", f, NUMLETT+1);
//			ArrayUtil::outputArray("max  key: ", fMaxLogscore, NUMLETT+1);
//		}
		// use the function which maximize the log score as the decryption function
		ArrayUtil::copyArray(keyMaxLogscore, testkey, NUMLETT+1);
	}

	// output decrypt function.
	cout << "Decrypt function: ";
	for (int i=0; i<NUMLETT; i++) {
		cout << ConvUtil::intIndexToChar(testkey[i]);
	}
	cout << endl;

    decryptedText = decryptByKey(ciphertext, testkey);

    lastresult.decryptionkey = keyMaxLogscore;
    lastresult.logscore = maxLogScore;

    return decryptedText;
}

double SubstitutionCipher::thelogscoreForPairFrequency(int traincounts[NUMLETT+1][NUMLETT+1], int testcounts[NUMLETT+1][NUMLETT+1], int f[NUMLETT+1]) {
    int i,j;
    double tmpscore = 0.0;
    for (i=0; i<=NUMLETT; i++) {
        for (j=0; j<=NUMLETT; j++) {
        	tmpscore = tmpscore
        	+ testcounts[i][j] * log(traincounts[f[i]][f[j]]);
        }
    }
    return(tmpscore * PIPOWER);
}

void SubstitutionCipher::countPairFrequency(string text, int pairFreqency[NUMLETT+1][NUMLETT+1]) {

	// initialize array
	for(int i=0; i<27; i++) {
		for (int j=0; j<27; j++) {
			pairFreqency[i][j] = 1;
		}
	}

	int prevnum = ConvUtil::charToIntIndex(text.at(0));
	int newnum;
	// calculate pair frequencies
	for (int i=1; i<text.length(); i++) {
		newnum = ConvUtil::charToIntIndex(text.at(i));
		//cout << "pairFreq[" << prevnum << "," << newnum << "]++" << endl;
		pairFreqency[prevnum][newnum] ++;
		prevnum = newnum;
	}
	return;
}


// ==============================


void SubstitutionCipher::countSingleFrequency(string text, int singleFrequency[27]) {
	// initialize array
	for(int i=0; i<27; i++) {
		singleFrequency[i] = 1;
	}

	// calculate single letter frequencies
	for (int i=0; i<text.length(); i++) {
		singleFrequency[ ConvUtil::charToIntIndex(text.at(i)) ] ++;
	}
	return;
}

string SubstitutionCipher::decryptUsingSingleFrequencyOrderAndMatch(string ciphertext, string trainingtext) {
	int trainingTextSingleFreq[27];
	int cipherTextSingleFreq[27];

	// calculate single letter frequencies for training text and cipher text
	countSingleFrequency(trainingtext, trainingTextSingleFreq);
	countSingleFrequency(ciphertext,   cipherTextSingleFreq);

	// calculate index order of training text and cipher text
	int trainingTextOrder[27];
	SortUtil::getDescendingOrder(trainingTextSingleFreq, trainingTextOrder, 27);
	int cipherTextOrder[27];
	SortUtil::getDescendingOrder(cipherTextSingleFreq, cipherTextOrder, 27);

	// the decrypt function is the mapping from trainingTextOrder to cipherTextOrder
	// construct decipher function
	int f[27];
	for (int i=0; i<27; i++) {
		f[ cipherTextOrder[i] ] = trainingTextOrder[i];
	}

//	ArrayUtil::outputArray("trainingTextSingleFreq", trainingTextSingleFreq, 27);
//	ArrayUtil::outputArray("trainingTextOrder", trainingTextOrder, 27);
//	ArrayUtil::outputArray("cipherTextSingleFreq",   cipherTextSingleFreq,   27);
//	ArrayUtil::outputArray("cipherTextOrder",   cipherTextOrder,   27);
//	ArrayUtil::outputArray("decypt function f", f, 27);

	// output decrypt function.
	cout << "Decrypt function: ";
	for (int i=0; i<NUMLETT; i++) {
		cout << ConvUtil::intIndexToChar(f[i]);
	}
	cout << endl;

	// decrypt
	string decryptedText = ciphertext;
	for (int i=0; i<decryptedText.length(); i++) {
		if (ciphertext[i] != ' ') {
			decryptedText[i] = 'A' + f[ConvUtil::charToIntIndex(ciphertext[i])];
		}
	}
	return decryptedText;

	//cout << "decyphered text: " << decryptedText.substr(0, 50) << endl;
}


string SubstitutionCipher::decryptUsingSingleFrequency(string ciphertext, string trainingtext) {
	string decryptedText = ciphertext;
	int trainingTextSingleFreq[NUMLETT+1];
	int testTextSingleFreq[NUMLETT+1];
	int f[NUMLETT+1], newf[NUMLETT+1];
	RandUtil randUtil;
	int a, b;
	countSingleFrequency(trainingtext, trainingTextSingleFreq);
	countSingleFrequency(ciphertext, testTextSingleFreq);

    /* Initialize decoding function f. */
    for (int i=0; i<(NUMLETT+1); i++) {
    	f[i] = i;
    }
    double prevlogscore = thelogscoreForSingleFrequency(trainingTextSingleFreq, testTextSingleFreq, f);

	int maxNoIteration = 100000;
	int outputInterval = maxNoIteration/5;

	for (int i=0; i<maxNoIteration; i++) {
		// propose newf by swapping 2 positions of f
		a = randUtil.newIntRand(NUMLETT+1);
		b = randUtil.newIntRand(NUMLETT+1);
		for (int j=0; j<(NUMLETT+1); j++) {
			newf[j] = f[j];
		}
		newf[a] = f[b];
		newf[b] = f[a];

		double newlogscore = thelogscoreForSingleFrequency(trainingTextSingleFreq, testTextSingleFreq, newf);
		if (i%outputInterval == 0) cout << "iteration: " << setw(4) << i << " swapping:" << setw(2) << a << "," << setw(2) << b << " prevlogscore: " << fixed << setprecision(0) << prevlogscore << " newlogscore: " << newlogscore;
		if ( log(randUtil.newRand()) < newlogscore - prevlogscore ) {
			if (i%outputInterval ==0) cout << " accepted";
		    /* Accept the new function. */
			for (int j=0; j<=(NUMLETT+1); j++) {
				f[j] = newf[j];
			}
			// output the key
			if (i%outputInterval ==0) {
				//ArrayUtil::outputArray("key:", f, 27);
			}
		    prevlogscore = newlogscore;
		} else {
		    /* Reject the new function. */
			if (i%outputInterval ==0) cout << " rejected";
		}
		if (i%outputInterval ==0) cout << endl;
	}


	// output decrypt function.
	cout << "Decrypt function: ";
	for (int i=0; i<NUMLETT; i++) {
		cout << char(f[i]+'A');
	}
	cout << endl;

	// constructing decrypted text:
	for (int i=0; i<ciphertext.length(); i++) {
		if (ciphertext[i] != ' ') {
			decryptedText[i] = 'A' + f[ ConvUtil::charToIntIndex(ciphertext[i])];
		}
	}

	return decryptedText;

}

double SubstitutionCipher::thelogscoreForSingleFrequency(int traincounts[NUMLETT+1], int testcounts[NUMLETT+1], int f[NUMLETT+1]) {
    int i;
    double tmpscore = 0.0;
    for (i=0; i<=NUMLETT; i++) {
		tmpscore = tmpscore
		+ testcounts[i] * log(traincounts[f[i]]);
    }
    return(tmpscore * PIPOWER);

}


// ==============================


void SubstitutionCipher::countTripleFrequency(string text, int tripleFreqency[NUMLETT+1][NUMLETT+1][NUMLETT+1]) {
	// initialization
	for (int i=0; i<(NUMLETT+1); i++) {
		for (int j=0; j<(NUMLETT+1); j++) {
			for (int k=0; k<(NUMLETT+1); k++) {
				tripleFreqency[i][j][k] = 1;
			}
		}
	}

	int charIndex[2];
	charIndex[0] = ConvUtil::charToIntIndex(text[0]);
	charIndex[1] = ConvUtil::charToIntIndex(text[1]);

	for (int i=2; i<text.length(); i++) {
		charIndex[2] = ConvUtil::charToIntIndex(text[i]);
		tripleFreqency[charIndex[0]][charIndex[1]][charIndex[2]] ++;

		// move on
		charIndex[0] = charIndex[1];
		charIndex[1] = charIndex[2];
	}
}

double SubstitutionCipher::thelogscoreForTripleFrequency(int traincounts[NUMLETT+1][NUMLETT+1][NUMLETT+1], int testcounts[NUMLETT+1][NUMLETT+1][NUMLETT+1], int f[NUMLETT+1]) {
    double tmpscore = 0.0;
    for (int i=0; i<=NUMLETT; i++) {
        for (int j=0; j<=NUMLETT; j++) {
        	for (int k=0; k<NUMLETT; k++) {
            	tmpscore = tmpscore
            	+ testcounts[i][j][k] * log(traincounts[f[i]][f[j]][f[k]]);
        	}
        }
    }
    return(tmpscore * PIPOWER);
}

string SubstitutionCipher::decryptUsingTripleFrequency(string ciphertext, string trainingtext, int numIteration) {
	string decryptedText = ciphertext;
	int trainingTextPairFreq[NUMLETT+1][NUMLETT+1][NUMLETT+1];
	int testTextPairFreq[NUMLETT+1][NUMLETT+1][NUMLETT+1];
	int f[NUMLETT+1], newf[NUMLETT+1];
	RandUtil randUtil;
	int a, b;
	countTripleFrequency(trainingtext, trainingTextPairFreq);
	countTripleFrequency(ciphertext, testTextPairFreq);

    /* Initialize decoding function f. */
    for (int i=0; i<(NUMLETT+1); i++) {
    	f[i] = i;
    }
    double prevlogscore = thelogscoreForTripleFrequency(trainingTextPairFreq, testTextPairFreq, f);

	int outputInterval = numIteration/5;
	int numAccept = 0;

	for (int i=0; i<numIteration; i++) {
		// propose newf by swapping 2 positions of f
		a = randUtil.newIntRand(NUMLETT+1);
		b = randUtil.newIntRand(NUMLETT+1);
		for (int j=0; j<(NUMLETT+1); j++) {
			newf[j] = f[j];
		}
		newf[a] = f[b];
		newf[b] = f[a];

		double newlogscore = thelogscoreForTripleFrequency(trainingTextPairFreq, testTextPairFreq, newf);
		if (i%outputInterval == 0) {
			cout << "iteration: " << setw(4) << i << " swapping:" << setw(2) << a << "," << setw(2) << b << " prevlogscore: " << fixed << setprecision(0) << prevlogscore << " newlogscore: " << newlogscore;
			cout << " text: " << decryptByKey(ciphertext.substr(0, 40), f) << ".";
		}
		if ( log(randUtil.newRand()) < newlogscore - prevlogscore ) {
			if (i%outputInterval == 0) cout << " accepted";
		    /* Accept the new function. */
			for (int j=0; j<=(NUMLETT+1); j++) {
				f[j] = newf[j];
			}
		    prevlogscore = newlogscore;
		    numAccept ++;
		} else {
		    /* Reject the new function. */
			if (i%outputInterval == 0) cout << " rejected";
		}
		if (i%outputInterval == 0) cout << endl;
	}

	cout << "Acceptance rate: " << (numAccept*1.0/numIteration) << endl;


	// output decrypt function.
	cout << "Decrypt function: ";
	for (int i=0; i<NUMLETT; i++) {
		cout << char(f[i]+'A');
	}
	cout << endl;

	// constructing decrypted text:
	for (int i=0; i<ciphertext.length(); i++) {
		if (ciphertext[i] != ' ') {
			decryptedText[i] = ConvUtil::intIndexToChar( f[ConvUtil::charToIntIndex(ciphertext[i])] );
		}
	}

	return decryptedText;

}

double SubstitutionCipher::verify(string plaintext, string decrypttext) {
	int textlength = decrypttext.length();
	plaintext = ConvUtil::convertToUpperCase(plaintext).substr(0, textlength);
	decrypttext = ConvUtil::convertToUpperCase(decrypttext);
	int noCorrectLetters = 0;
	int noAvailableLetters = 26;

	for (int i=0; i<NUMLETT; i++) {
		// check first occurrence of each letters
		char lettertocheck = ConvUtil::intIndexToChar(i);
		int firstOccurInPlaintext = plaintext.find_first_of(lettertocheck, 0);
		int firstOccurInDecrypttext = decrypttext.find_first_of(lettertocheck, 0);

		if ( firstOccurInPlaintext == -1 ) { // if not found in plain text, remove from available letters
			noAvailableLetters --;
		} else if ( firstOccurInPlaintext == firstOccurInDecrypttext ) {	// same first position in plain text and decrypted text, correct
			noCorrectLetters ++;
		} else {
			//cout << "incorrect: " << lettertocheck << " " << firstOccurInPlaintext << ", " << firstOccurInDecrypttext << endl;
		}
	}
	cout << "Correct letters: " << noCorrectLetters << " out of " << noAvailableLetters << ". Percentage: " << setprecision(4) << (1.0*noCorrectLetters/noAvailableLetters) << endl;
	return (1.0*noCorrectLetters/noAvailableLetters);

}

string SubstitutionCipher::decryptUsingPairFrequencyWithRepetition(string ciphertext, string trainingtext, int numIteration) {
	int trainingTextPairFreq[NUMLETT+1][NUMLETT+1];
	int testTextPairFreq[NUMLETT+1][NUMLETT+1];
	int numRepetition = 5;

	RandUtil* randUtil = new RandUtil();

	double bestlogscore = 0;
	string bestdecryptedtext;

	for (int i=0; i<numRepetition; i++) {
		cout << endl << "Repetition " << i+1 << "/" << numRepetition << endl;

		// randomly choose 2000 text from the cipher text
		int randomposition = randUtil->newIntRand(ciphertext.length()-2000);
		string randomciphertext = ciphertext.substr(randomposition, 2000);

		// decrypt the chosen 2000 cipher text
		decryptUsingPairFrequency(randomciphertext, trainingtext, numIteration);

		// decrypt full text by the key
		string decryptedtext = decryptByKey(ciphertext, lastresult.decryptionkey);
		cout << "decrypted full text: " << decryptedtext.substr(0, 80) << endl;

		// calculate logscore for the full text
		countPairFrequency(trainingtext, trainingTextPairFreq);
		countPairFrequency(ciphertext, testTextPairFreq);

		double logscore = thelogscoreForPairFrequency(trainingTextPairFreq, testTextPairFreq, lastresult.decryptionkey);
		cout << "log score for the full text: " << logscore << endl;

		if (bestlogscore < logscore) {
			bestlogscore = logscore;
			bestdecryptedtext = decryptedtext;
		}

	}

	cout << endl << "best log score: " << bestlogscore << endl;

	return bestdecryptedtext;
}

string SubstitutionCipher::decrypt_FinalVersion(string ciphertext, string trainingtext, int numIteration) {
	int trainingTextPairFreq[NUMLETT+1][NUMLETT+1];
	int testTextPairFreq[NUMLETT+1][NUMLETT+1];
	int numRepetition = 10;

	RandUtil* randUtil = new RandUtil();

	double bestlogscore = 0;
	string bestdecryptedtext;

	for (int i=0; i<numRepetition; i++) {
		cout << endl << "Repetition " << i+1 << "/" << numRepetition << endl;

		// decrypt first with single frequency
		ciphertext = decryptUsingSingleFrequencyOrderAndMatch(ciphertext, trainingtext);
		cout << "decrypted text after uni-gram attack: " << ciphertext.substr(0, 80) << endl;

		// randomly choose 2000 text from the cipher text
		int randomposition = randUtil->newIntRand(ciphertext.length()-2000);
		string randomciphertext = ciphertext.substr(randomposition, 2000);

		// decrypt the chosen 2000 cipher text
		decryptUsingPairFrequency(randomciphertext, trainingtext, numIteration);

		// decrypt full text by the key
		string decryptedtext = decryptByKey(ciphertext, lastresult.decryptionkey);
		cout << "decrypted full text: " << decryptedtext.substr(0, 80) << endl;

		// calculate logscore for the full text
		countPairFrequency(trainingtext, trainingTextPairFreq);
		countPairFrequency(ciphertext, testTextPairFreq);

		double logscore = thelogscoreForPairFrequency(trainingTextPairFreq, testTextPairFreq, lastresult.decryptionkey);
		cout << "log score for the full text: " << logscore << endl;

		if (bestlogscore < logscore) {
			bestlogscore = logscore;
			bestdecryptedtext = decryptedtext;
		}

	}

	cout << endl << "best log score: " << bestlogscore << endl;

	return bestdecryptedtext;
}
