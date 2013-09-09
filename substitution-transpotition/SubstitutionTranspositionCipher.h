/*
 * SubstitutionTranspositionCipher.h
 *
 *  Created on: Mar 15, 2010
 *      Author: cinc
 */

#ifndef SUBSTITUTIONTRANSPOSITIONCIPHER_H_
#define SUBSTITUTIONTRANSPOSITIONCIPHER_H_

#include <string>
#include "../transposition/TranspositionCipher.h"
#include "../substitution/SubstitutionCipher.h"

using namespace std;

class SubstitutionTranspositionCipher {
private:
	TranspositionCipher* transpositionCipher;
	SubstitutionCipher* substitutionCipher;
public:
	SubstitutionTranspositionCipher();
	virtual ~SubstitutionTranspositionCipher();

	string decrypt(string ciphertext, string trainingtext, int transpositionkeylength, int numCycle, int numIterationSubstitution, int numIterationTransposition);

	string encrypt(string plaintext, int transpositionkey[], int transpositionkeylength);

	double verify(string plaintext, string decrypttext, int period);

};

#endif /* SUBSTITUTIONTRANSPOSITIONCIPHER_H_ */
