/*
 * PairSubstitutionCipher.h
 *
 *  Created on: Feb 21, 2010
 *      Author: cinc
 */

#ifndef PAIRSUBSTITUTIONCIPHER_H_
#define PAIRSUBSTITUTIONCIPHER_H_

#include "string"

using namespace std;

#define NUMLETT 26
#define PIPOWER 0.25

class PairSubstitutionCipher {
private:
public:
	/**
	 * encrypt a plaintext by using pair substitution, i.e. "AB" -> "KY" ...
	 */
	string encrypt(string plaintext);
	/**
	 * decrypt a pair-substitution-ciphertext by comparing pair frequencies of pairs of characters
	 */
	string decryptUsingPairFrequency(string ciphertext, string trainingtext);
	/**
	 * decrypt a pair-substitution-ciphertext by a decryption function
	 */
	string decrypt(string text, int f[NUMLETT*NUMLETT]);

	/**
	 * decrypt by comparing single frequencies of pairs of characters in ciphertext and trainingtext
	 */
	string decryptUsingSingleFrequency(string ciphertext, string trainingtext);

	double thelogscoreForPairFrequency(int traincounts[NUMLETT*NUMLETT][NUMLETT*NUMLETT], int testcounts[NUMLETT*NUMLETT][NUMLETT*NUMLETT], int f[NUMLETT*NUMLETT]);
	/**
	 * count pair frequencies of pair of characters.
	 * i.e. frequency['ab']['ad'] = 2
	 */
	void countPairFrequency(string text, int freqency[NUMLETT*NUMLETT][NUMLETT*NUMLETT]);
	/**
	 * count frequencies of pair of characters
	 * e.g. frequency['ab'] = 10
	 */
	void countSinglePairFrequency(string text, int frequency[NUMLETT*NUMLETT]);
};

#endif /* PAIRSUBSTITUTIONCIPHER_H_ */
