/*
 * SubstitutionTranspositionCipher.cpp
 *
 *  Created on: Mar 15, 2010
 *      Author: cinc
 */

#include "SubstitutionTranspositionCipher.h"
#include <iostream>
#include <string>
#include "../transposition/TranspositionCipher.h"
#include "../substitution/SubstitutionCipher.h"

using namespace std;

SubstitutionTranspositionCipher::SubstitutionTranspositionCipher() {
	transpositionCipher = new TranspositionCipher();
	substitutionCipher = new SubstitutionCipher();
}

SubstitutionTranspositionCipher::~SubstitutionTranspositionCipher() {
	delete(transpositionCipher);
	delete(substitutionCipher);
}

string SubstitutionTranspositionCipher::decrypt(string ciphertext, string trainingtext, int transpositionkeylength, int numCycle, int numIterationSubstitution, int numIterationTransposition) {
    cout << "cipher text: " << ciphertext.substr(0, 80) << endl;

    cout << endl << "Step 0: " << "using substitution cipher (single letter frequency)" << endl;
    string decryptedtext0 = substitutionCipher->decryptUsingSingleFrequencyOrderAndMatch(ciphertext, trainingtext);
    cout << "decrypted text 0: " << decryptedtext0.substr(0, 80) << endl;
//    string decryptedtext0 = ciphertext;

    string decryptedtext1, decryptedtext2;

	for (int i=0; i<numCycle; i++) {
		cout << endl << "Cycle: " << i+1 << "/" << numCycle << endl;

		cout << "Step 1: " << "using transposition cipher (pair letter frequency)" << endl;
		decryptedtext1 = transpositionCipher->decryptUsingPairFrequency(decryptedtext0, trainingtext, transpositionkeylength, numIterationTransposition);
		cout << "decrypted text 1: " << decryptedtext1.substr(0, 80) << endl;

		cout << "Step 2: " << "using substitution cipher (pair letter frequency)" << endl;
		decryptedtext2 = substitutionCipher->decryptUsingPairFrequency(decryptedtext1, trainingtext, numIterationSubstitution);
		cout << "decrypted text 2: " << decryptedtext2.substr(0, 80) << endl;

		decryptedtext0 = decryptedtext2;
	}
    return decryptedtext2;
}

string SubstitutionTranspositionCipher::encrypt(string plaintext, int transpositionkey[], int transpositionkeylength) {
    string ciphertext1 = substitutionCipher->encrypt(plaintext);
    // temporary
    cout << "temp ciphertext1: " << ciphertext1.substr(0, 80) << endl;

    string ciphertext = transpositionCipher->encrypt(ciphertext1, transpositionkey, transpositionkeylength);
    cout << "temp ciphertext: " << ciphertext.substr(0, 80) << endl;

    return ciphertext;
}

double SubstitutionTranspositionCipher::verify(string plaintext, string decrypttext, int period) {
	return transpositionCipher->verify(plaintext, decrypttext, period);
}
