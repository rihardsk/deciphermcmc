/*
 * TranspositionCipher.h
 *
 *  Created on: Jan 31, 2010
 *      Author: cinc
 */

#ifndef TRANSPOSITIONCIPHER_H_
#define TRANSPOSITIONCIPHER_H_

#include <string>
using namespace std;

class TranspositionCipher {
private:
	double thelogscore(int trainingTextPairFreq[27][27], int testTextPairFreq[27][27]);
    void swapKey(int testKey[], int testNewKey[], int keylength, int pos1, int pos2);

    struct Result {
    	int keylength;
    	int* decryptionkey;
    	double logscore;
    } lastresult;
public:
    /**
     * move a single letter from position 1 to position 2
     * input:  key[]
     * output: newKey[]
     *
     * e.g. key = [01234567], pos1 = 3, pos2 = 6;
     *  =>  newKey = [01245637]
     */
    void slideMove(int* key, int* newKey, int keylength, int pos1, int pos2);
    /**
     * move a block of integers from position 1 to position 2
     * input:  key[]
     * output: newKey[]
     */
    void blockSlideMove(int *key, int* newKey, int keylength, int pos1, int pos2, int blocksize);
	void countPairFrequency(string text, int pairFreqency[27][27]);

	/**
	 * encryption by transposition (swapping positions) with a key
	 *
	 * the value of the key:
	 *
	 * e.g. if the key is "3021", the plaintext "abcdabcd" will be encrypt to "dacbdacb"
	 *
	 * Note: this method only support a key with a maximum length 10
	 *       for key with length greater than 10, use the other encrypt method with int[] key
	 */
	string encrypt(string plaintext, string key);

	/**
	 * encrypt by transposition (swapping positions) with a longer key
	 */
	string encrypt(string plaintext, int key[], int keylength);

	/**
	 * decrypt by comparing pair letter frequencies of training text and cipher text
	 */
	string decryptUsingPairFrequency(string ciphertext, string trainingtext, int keylength, int numIteration);

	/**
	 * decrypt when the key length is unknown
	 */
	string decryptWithUnknownKeyLength(string ciphertext, string trainingtext, int ciphertextlength, int minkeylength, int maxkeylength);

	/**
	 * decrypt by key
	 */
	string decryptByKey(string ciphertext, int key[], int keylength);
	string decryptByKey(string ciphertext, string key);

	string decrypt_FinalVersion(string ciphertext, string trainingtext, int ciphertextlength, int keylength, int numIteration);

	/**
	 * verify and output the correctness of the decrypt text
	 *
	 * a letter is said to be correctly placed if its neighborhoods are the same as the neighbors for the correct key
	 *
	 */
	double verify(string plaintext, string decrypttext, int period);

};

#endif /* TRANSPOSITIONCIPHER_H_ */
