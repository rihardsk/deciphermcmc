/*
 * SubstitutionCipher.h
 *
 *  Created on: Jan 28, 2010
 *      Author: cinc
 */

#ifndef SUBSTITUTIONCIPHER_H_
#define SUBSTITUTIONCIPHER_H_

#include <string>
using namespace std;

#define NUMLETT 26
#define PIPOWER 0.25

class SubstitutionCipher {
private:
	/**
	 * calculate the log score for singleFrequency array
	 */
	double thelogscoreForSingleFrequency(int traincounts[NUMLETT+1], int testcounts[NUMLETT+1], int f[NUMLETT+1]);
	/**
	 * calculate the log score for pairFrequency array
	 */
	double thelogscoreForPairFrequency(int traincounts[NUMLETT+1][NUMLETT+1], int testcounts[NUMLETT+1][NUMLETT+1], int f[NUMLETT+1]);
	/**
	 * calculate the log score for tripleFrequency array
	 */
	double thelogscoreForTripleFrequency(int traincounts[NUMLETT+1][NUMLETT+1][NUMLETT+1], int testcounts[NUMLETT+1][NUMLETT+1][NUMLETT+1], int f[NUMLETT+1]);

	void countSingleFrequency(string text, int singleFrequency[NUMLETT+1]);
	void countPairFrequency  (string text, int pairFreqency[NUMLETT+1][NUMLETT+1]);
	void countTripleFrequency(string text, int tripleFreqency[NUMLETT+1][NUMLETT+1][NUMLETT+1]);
    string decryptByKey(string ciphertext, int f[NUMLETT + 1]);

    struct Result {
    	int* decryptionkey;
    	double logscore;
    } lastresult;

public:
	string encrypt(string plaintext);

	string decryptUsingSingleFrequencyOrderAndMatch(string ciphertext, string trainingtext);
	string decryptUsingSingleFrequency(string ciphertext, string trainingtext);
	string decryptUsingPairFrequency  (string ciphertext, string trainingtext, int numIteration);
	string decryptUsingTripleFrequency(string ciphertext, string trainingtext, int numIteration);
	string decryptUsingPairFrequencyWithRepetition(string ciphertext, string trainingtext, int numIteration);

	/**
	 * Final version
	 * steps:
	 * 	5 repetitions, each repetition, decrypt 2000 cipher text from different position of the full cipher text
	 *  then calculate the log score. use the key which maximize the log score
	 */
	string decrypt_FinalVersion(string ciphertext, string trainingtext, int numIteration);

	/**
	 * verify and output the correctness of the decrypt text
	 * by checking the correctness of 26 English characters
	 *
	 * return percentage of the correctness
	 */
	double verify(string plaintext, string decrypttext);
};

#endif /* SUBSTITUTIONCIPHER_H_ */
