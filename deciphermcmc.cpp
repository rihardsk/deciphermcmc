//============================================================================
// Name	       : deciphermcmc.cpp
// Author      : Jian Chen 'jchen (at) utstat.toronto.edu'
// Version     :
// Copyright   : Decipher MCMC Program
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <iomanip>
#include "common/RandUtil.h"
#include "common/TextFileUtil.h"
#include "common/SortUtil.h"
#include "common/ArrayUtil.h"
#include "common/ConvUtil.h"
#include "common/SysUtil.h"
#include "transposition/TranspositionCipher.h"
#include "substitution/SubstitutionCipher.h"
#include "substitution/PairSubstitutionCipher.h"
#include "substitution-transpotition/SubstitutionTranspositionCipher.h"

using namespace std;

void test()
{
	cout << "!!!Hello World!!!" << endl; // prints !!!Hello World!!!
	RandUtil *randUtil = new RandUtil();
	cout << randUtil->newIntRand(26) << endl;

	cout << randUtil->newRand() << endl;
}

void testSysUtil() {
	cout << fixed << SysUtil::getCurrentTimestamp() << endl;

	// calculate elapsed time
	double startTimestamp = SysUtil::getCurrentTimestamp();
	for (int i=0; i<100000000; i++) {

	}
	double elapsedTime = SysUtil::calculateElapsedTime(startTimestamp);
	cout << "Elapsed seconds: " << elapsedTime << " second(s)." << endl;
}

void testTranspositionCipher()
{
	RandUtil* randUtil = new RandUtil();
	// test transposition cipher
	cout << "test transposition cipher" << endl;
	TranspositionCipher *trCipher = new TranspositionCipher();
//	string plaintext = "12345234563456745678";
//	string key = "34125";
//	cout << "plaintext:  " << plaintext << endl;
//	cout << "key:        " << key << endl;
//	string ciphertext = trCipher->encrypt(plaintext, key);
//	cout << "ciphertext: " << ciphertext << endl;
//	cout << trCipher->encrypt("abcde12345", "34125") << endl;
	//int pairFreq[27][27];
	//trCipher->countPairFrequency("ababzz..", pairFreq);
	//cout << "pair freq" << pairFreq[0][1] << endl;

	// tests for verify()
//	cout << endl;
//	cout << trCipher->verify("01234", "01234", 5) << endl;
//	cout << trCipher->verify("01234", "12340", 5) << endl;
//	cout << trCipher->verify("01234", "01243", 5) << endl;
//	cout << trCipher->verify("01234", "12034", 5) << endl;

//	string plaintext = "01234";
//	string key = "21430";
//	string ciphertext = trCipher->encrypt(plaintext, key);
//	string decryptedtext = trCipher->decryptByKey(ciphertext, key);
//	cout << "ciphertext: " << ciphertext << endl;
//	cout << "decryptedtext: " << decryptedtext << endl;

	// encrypt oliver text
	string plaintext = TextFileUtil::readTextFromFile("oliverorig");
	//string oliverCipherText = trCipher->encrypt(oliverPlainText, "3579218460");
	string trainingtext = TextFileUtil::readTextFromFile("warorig");


	int noOfRun = 100;
	int keylength = 20;
	int numIteration = 10*1000;
	int ciphertextlength = 1*1000;

	double correctness[noOfRun];

	for (int i=0; i<noOfRun; i++) {
		cout << endl << "Run " << i+1 << "/" << noOfRun << endl;

		// encrypt
		int* key = randUtil->shuffle(keylength);
		string oliverciphertext = trCipher->encrypt(plaintext, key, keylength);
		cout << "cipher text: " << oliverciphertext.substr(0, 80) << endl;
		oliverciphertext = oliverciphertext.substr(0, ciphertextlength);

		double startTime = SysUtil::getCurrentTimestamp();

		// decrypt
		string decrypttext = trCipher->decryptUsingPairFrequency(oliverciphertext, trainingtext, keylength, numIteration);
//		string decrypttext = trCipher->decrypt_FinalVersion(oliverciphertext, trainingtext, ciphertextlength, keylength, numIteration);

		cout << "Elapsed time in seconds: " << setprecision(6) << SysUtil::calculateElapsedTime(startTime) << endl;
		cout << "decrypted text: " << decrypttext.substr(0, 80) << endl;

		correctness[i] = trCipher->verify(plaintext, decrypttext, keylength);
	}
	ArrayUtil::outputArray("Correctness: ", correctness, noOfRun);
	cout << "Average correctness: " << ArrayUtil::average(correctness, noOfRun) << endl;
	cout << "Average no of correct positions: " << (keylength*ArrayUtil::average(correctness, noOfRun)) << endl;
	cout << "No of successful decryptions: " << ArrayUtil::countNoOfOccur(1.00, correctness, noOfRun) << endl;

}

void testTranspositionCipher_UnknownKeyLength()
{
	RandUtil* randUtil = new RandUtil();
	// test transposition cipher
	cout << "test transposition cipher for unknown key length" << endl;
	TranspositionCipher *trCipher = new TranspositionCipher();

	// encrypt oliver text
	string plaintext = TextFileUtil::readTextFromFile("oliverorig");
	//string oliverCipherText = trCipher->encrypt(oliverPlainText, "3579218460");
	string trainingtext = TextFileUtil::readTextFromFile("warorig");


	int noOfRun = 10;
	int minkeylength = 20;
	int maxkeylength = 30;
	int ciphertextlength = 2000;

	double correctness[noOfRun];

	for (int i=0; i<noOfRun; i++) {
		cout << endl << "Run " << i+1 << "/" << noOfRun << endl;

		// encrypt
		int keylength = randUtil->newIntRand(maxkeylength-minkeylength) + minkeylength;
		cout << "key length: " << keylength << endl;

		int* key = randUtil->shuffle(keylength);
		string oliverciphertext = trCipher->encrypt(plaintext, key, keylength);
		cout << "cipher text: " << oliverciphertext.substr(0, 80) << endl;
		oliverciphertext = oliverciphertext.substr(0, ciphertextlength);

		double startTime = SysUtil::getCurrentTimestamp();

		// decrypt
		string decrypttext = trCipher->decryptWithUnknownKeyLength(oliverciphertext, trainingtext, ciphertextlength, minkeylength, maxkeylength);


		cout << "Elapsed time in seconds: " << setprecision(6) << SysUtil::calculateElapsedTime(startTime) << endl;
		cout << "decrypted text: " << decrypttext.substr(0, 80) << endl;

		correctness[i] = trCipher->verify(plaintext, decrypttext, keylength);
	}
	ArrayUtil::outputArray("Correctness: ", correctness, noOfRun);
	cout << "Average correctness: " << ArrayUtil::average(correctness, noOfRun) << endl;
//	cout << "Average no of correct positions: " << (keylength*ArrayUtil::average(correctness, noOfRun)) << endl;
	cout << "No of successful decryptions: " << ArrayUtil::countNoOfOccur(1.00, correctness, noOfRun) << endl;

}

void testTranspositionCipher_DifferentPartOfCipherText()
{
	RandUtil* randUtil = new RandUtil();
	cout << "test transposition cipher" << endl;
	TranspositionCipher *trCipher = new TranspositionCipher();

	// encrypt oliver text
	string oliverPlainText = TextFileUtil::readTextFromFile("oliverorig");
	string trainingtext = TextFileUtil::readTextFromFile("wrnpc12.txt");


	int noOfRun = 100;
	int keylength = 20;
	int numIteration = 10*1000;
	int ciphertextlength = 2*1000;
	int startposition = randUtil->newIntRand(oliverPlainText.length()-keylength) /20 * 20;

	double correctness[noOfRun];

	for (int i=0; i<noOfRun; i++) {
		cout << endl << "Run " << i+1 << "/" << noOfRun << endl;

		// encrypt
		int* key = randUtil->shuffle(keylength);
		string oliverciphertext = trCipher->encrypt(oliverPlainText, key, keylength);
		cout << "cipher text: " << oliverciphertext.substr(0, 80) << endl;
		oliverciphertext = oliverciphertext.substr(startposition, ciphertextlength);

		double startTime = SysUtil::getCurrentTimestamp();

		// decrypt
		string decrypttext = trCipher->decryptUsingPairFrequency(oliverciphertext, trainingtext, keylength, numIteration);

		cout << "Elapsed time in seconds: " << setprecision(6) << SysUtil::calculateElapsedTime(startTime) << endl;
		cout << "decrypted text: " << decrypttext.substr(0, 80) << endl;

		correctness[i] = trCipher->verify(oliverPlainText.substr(startposition), decrypttext, keylength);
	}
	cout << "cipher text start position: " << startposition << endl;
	ArrayUtil::outputArray("Correctness: ", correctness, noOfRun);
	cout << "Average correctness: " << ArrayUtil::average(correctness, noOfRun) << endl;
	cout << "Average no of correct positions: " << (keylength*ArrayUtil::average(correctness, noOfRun)) << endl;
	cout << "No of successful decryptions: " << ArrayUtil::countNoOfOccur(1.00, correctness, noOfRun) << endl;

}
void testTranspositionCipher_repetition() {
	RandUtil* randUtil = new RandUtil();
	// test transposition cipher
	cout << "test transposition cipher" << endl;
	TranspositionCipher *trCipher = new TranspositionCipher();

	// encrypt oliver text
	string oliverPlainText = TextFileUtil::readTextFromFile("oliverorig");
//	string oliverCipherText = trCipher->encrypt(oliverPlainText, "3579218460");

	string trainingtext = TextFileUtil::readTextFromFile("wrnpc12.txt");

	int noOfRun = 10;
	int keylength = 20;
	int numIteration = 10*1000;
	int noOfParallel = 5;
	int ciphertextlength = 2*1000;

	cout << endl;
	double correctness[noOfRun];
	for (int i=0; i<noOfRun; i++) {
		cout << endl << "Run " << i+1 << "/" << noOfRun << endl;

		correctness[i] = 0;
		for (int j=0; j<noOfParallel; j++) {
			cout << endl << "Repetition #" << j << ":" << endl;
			// encrypt
			int* key = randUtil->shuffle(keylength);
			string oliverciphertext = trCipher->encrypt(oliverPlainText, key, keylength);
			oliverciphertext = oliverciphertext.substr(0, ciphertextlength);

			// decrypt
			string decrypttext = trCipher->decryptUsingPairFrequency(oliverciphertext, trainingtext, keylength, numIteration);
			cout << "decrypted text: " << decrypttext.substr(0, 80) << endl;

			double correctnessThisParallelRun = trCipher->verify(oliverPlainText, decrypttext, keylength);
			if (correctnessThisParallelRun > correctness[i]) {
				correctness[i] = correctnessThisParallelRun;
			}
		}

	}
	ArrayUtil::outputArray("Correctness: ", correctness, noOfRun);
	cout << "Average correctness: " << ArrayUtil::average(correctness, noOfRun) << endl;
	cout << "Average no of correct positions: " << (keylength*ArrayUtil::average(correctness, noOfRun)) << endl;
	cout << "No of successful decryptions: " << ArrayUtil::countNoOfOccur(1.00, correctness, noOfRun) << endl;

}



void testSubstitutionCipher() {
	SubstitutionCipher* sCipher = new SubstitutionCipher();
	//string decryptedText = sCipher->decrypt("BCDEF", "ABCDEABCDEABCDEABCDEABCDE");
	//cout << "decrypted text: " << decryptedText << endl;

	string oliverplaintext = TextFileUtil::readTextFromFile("hockeyorig");

	string trainingtext = TextFileUtil::readTextFromFile("oliverorig");

	// initialize parameters
	int noOfRun = 100;
	int numIteration = 10*1000;
	int ciphertextlength = 2000;

	double correctness[noOfRun];
	for (int i=0; i<noOfRun; i++) {
		cout << endl << "Run " << i+1 << "/" << noOfRun << endl;

		// encrypt
		string oliverciphertext = sCipher->encrypt(oliverplaintext);
//		oliverciphertext = oliverciphertext.substr(0, ciphertextlength);
		cout << "oliverciphertext: " << oliverciphertext.substr(0, 80) << endl;

		double startTime = SysUtil::getCurrentTimestamp();

//		string decryptedText = sCipher->decryptUsingSingleFrequencyOrderAndMatch(oliverciphertext, trainingtext);
//		string decryptedText = sCipher->decryptUsingSingleFrequency(oliverciphertext, trainingtext);
//		string decryptedText = sCipher->decryptUsingPairFrequency(oliverciphertext, trainingtext, numIteration);
//		string decryptedText = sCipher->decryptUsingTripleFrequency(oliverciphertext, trainingtext, numIteration);
//		string decryptedText = sCipher->decryptUsingPairFrequencyWithRepetition(oliverciphertext, trainingtext, numIteration);
		string decryptedText = sCipher->decrypt_FinalVersion(oliverciphertext, trainingtext, numIteration);

		TextFileUtil::writeTextToFile("oliver.substitution.decrypted.txt", decryptedText);
		cout << "oliver decrypted text: " << decryptedText.substr(0,80) << endl;

		cout << "Elapsed time in seconds: " << setprecision(6) << SysUtil::calculateElapsedTime(startTime) << endl;
		// verify the correctness of the decrypted text
		correctness[i] = sCipher->verify(oliverplaintext, decryptedText);
	}
	ArrayUtil::outputArray("Correctness", correctness, noOfRun);
	cout << "Average accuracy: " << ArrayUtil::average(correctness, noOfRun) << endl;
	cout << "No of successful decryptions: " << ArrayUtil::countNoOfOccur(1.00, correctness, noOfRun) << endl;

}


void testSubstitutionCipher_DifferentPartOfCipherText() {
	SubstitutionCipher* sCipher = new SubstitutionCipher();

	string oliverplaintext = TextFileUtil::readTextFromFile("oliverorig");

	string trainingtext = TextFileUtil::readTextFromFile("wrnpc12.txt");

	int noOfRun = 1;
	int numIteration = 10*1000;
	int ciphertextlength = 2000;

	RandUtil* randUtil = new RandUtil();
	int startPosition = randUtil->newIntRand(oliverplaintext.length()-2000);
	cout << "cipher text start position: " << startPosition << endl;
//	int startPosition = oliverplaintext.length()-2000;	// last 2000

	double correctness[noOfRun];
	for (int i=0; i<noOfRun; i++) {
		cout << endl << "Run " << i+1 << "/" << noOfRun << endl;

		// encrypt
		string oliverciphertext = sCipher->encrypt(oliverplaintext);
		oliverciphertext = oliverciphertext.substr(startPosition, ciphertextlength);
		cout << "oliverciphertext: " << oliverciphertext.substr(0, 80) << endl;

		double startTime = SysUtil::getCurrentTimestamp();

		// decrypt using pair frequency
		string decryptedText = sCipher->decryptUsingPairFrequency(oliverciphertext, trainingtext, numIteration);
		TextFileUtil::writeTextToFile("oliver.substitution.decrypted.txt", decryptedText);
		cout << "oliver decrypted text: " << decryptedText.substr(0,80) << endl;

		cout << "Elapsed time in seconds: " << setprecision(6) << SysUtil::calculateElapsedTime(startTime) << endl;
		// verify the correctness of the decrypted text
		correctness[i] = sCipher->verify(oliverplaintext.substr(startPosition, ciphertextlength), decryptedText);

//		cout << "----decrypted    text: " << decryptedText << endl;
//		cout << "----oliver plain text: " << oliverplaintext.substr(startPosition, ciphertextlength) << endl;
	}
	cout << "cipher text start position: " << startPosition << endl;
	ArrayUtil::outputArray("Correctness", correctness, noOfRun);
	cout << "Average accuracy: " << ArrayUtil::average(correctness, noOfRun) << endl;
	cout << "No of successful decryptions: " << ArrayUtil::countNoOfOccur(1.00, correctness, noOfRun) << endl;

}

void testTextFileUtil() {
	//string oliverText = TextFileUtil::readTextFromFile("oliverorig");
	//cout << oliverText << endl;
	//TextFileUtil::writeTextToFile("oliver.copy", oliverText);

	// get file size, read binary file
//	long size = TextFileUtil::getFileSize("oliverscram");
//	cout << size << endl;
//	char* c = TextFileUtil::readBinaryFile("oliverscram", size);
//	for (int i=0; i<10; i++) {
//		cout << c[i];
//	}
//	cout << endl;

	// get latin letter string
	string str1 = "The Project ... ? \n is ok.";
	string str2 = TextFileUtil::getLatinAlphabetString(str1);
	cout << "original text: " << str1 << endl;
	cout << "only alphabetic letters: " << str2 << endl;
}


void testSlideMove() {
	int keylength = 8;
	int key[]	= {0,1,2,3,4,5,6,7};
	int newKey[] = {0,1,2,3,4,5,6,7};
	TranspositionCipher trCipher;

	// single slide move
//	trCipher.slideMove(key, newKey, keylength, 3, 6);
//	trCipher.slideMove(key, newKey, keylength, 3, 7);
//	trCipher.slideMove(key, newKey, keylength, 0, 6);
//	trCipher.slideMove(key, newKey, keylength, 6, 3);
//	trCipher.slideMove(key, newKey, keylength, 6, 0);
//	trCipher.slideMove(key, newKey, keylength, 7, 3);

	// block slide move
//	trCipher.blockSlideMove(key, newKey, keylength, 3, 6, 2); // 01256347
//	trCipher.blockSlideMove(key, newKey, keylength, 3, 6, 3); // 01263457
//	trCipher.blockSlideMove(key, newKey, keylength, 3, 6, 4); // 3+4>6, no move
//	trCipher.blockSlideMove(key, newKey, keylength, 0, 6, 2); // 23456017
//	trCipher.blockSlideMove(key, newKey, keylength, 0, 7, 2); // 23456701
//	trCipher.blockSlideMove(key, newKey, keylength, 3, 7, 2); // 01256734
//	trCipher.blockSlideMove(key, newKey, keylength, 0, 7, 7); // 70123456
//	trCipher.blockSlideMove(key, newKey, keylength, 0, 7, 8); // no move

//	trCipher.blockSlideMove(key, newKey, keylength, 6, 3, 2); // 01267345
//	trCipher.blockSlideMove(key, newKey, keylength, 6, 0, 2); // 67012345
//	trCipher.blockSlideMove(key, newKey, keylength, 6, 3, 3); // 6+3>8, no move
//	trCipher.blockSlideMove(key, newKey, keylength, 1, 0, 6); // 12345607
//	trCipher.blockSlideMove(key, newKey, keylength, 1, 0, 7); // 12345670
//	trCipher.blockSlideMove(key, newKey, keylength, 7, 0, 1); // 70123456

//	trCipher.blockSlideMove(key, newKey, keylength, 3, 3, 2); // pos1 = pos2, no move

	for(int i=0; i<keylength; i++) { cout << newKey[i]; }
	cout << endl;
}


void testSortUtil() {
	int values[5] = { 5, 9, 1, 4, 6 };
	int order[5];
	SortUtil::getDescendingOrder(values, order, 5);
	cout << "orders: ";
	for (int i=0; i<5; i++) {
		cout << order[i];
	}
	cout << endl;
}

void testConvUtil() {
	cout << "c -> " << ConvUtil::charToIntIndex('c') << endl;
	char c[2]  = {'a','c'};
	cout << "ac ->" << ConvUtil::pairCharToIntIndex( c ) << endl;
	c[0] = 'b'; c[1] = 'c';
	cout << "bc -> " << ConvUtil::pairCharToIntIndex( c ) << endl;

	ConvUtil::intIndexToPairChar(29, c);
	cout << "29 -> " << c[0] << "," << c[1] << endl;

	cout << "Upper case: " << ConvUtil::convertToUpperCase("I am a student. ZZZzzz...") << endl;
}

void testShuffle() {
	RandUtil *randUtil = new RandUtil();

	//int* values = randUtil->shuffle(size);

	int* values = randUtil->fisherYatesShuffle(4);

	ArrayUtil::outputArray("Shuffle", values, 4);
}

void testPairSubstitutionCipher() {
	PairSubstitutionCipher *cipher = new PairSubstitutionCipher();

	//	string ciphertext = cipher->encrypt("ABCDEFGH  ABCDEFGH");
//	cout << "encrypt by pair substitution: " << ciphertext << endl;

	// encryption
//	string oliverPlainText = TextFileUtil::readTextFromFile("oliverorig");
//	oliverPlainText = TextFileUtil::getLatinAlphabetString(oliverPlainText);
//	string ciphertext = cipher->encrypt(oliverPlainText);
//	TextFileUtil::writeTextToFile("oliver.pairsubstitution.ciphertext", ciphertext);

	string trainingtext = TextFileUtil::readTextFromFile("wrnpc12.txt");
	trainingtext = TextFileUtil::getLatinAlphabetString(trainingtext);
	cout << "training text: " << trainingtext.substr(0, 100) << endl;

	string oliverciphertext = TextFileUtil::readTextFromFile("oliver.pairsubstitution.ciphertext");
	cout << "cipher text: " << oliverciphertext.substr(0, 100) << endl;

//	oliverciphertext = oliverciphertext.substr(0, 10000);

	// decrypt using single frequencies
	string decryptedtext = cipher->decryptUsingSingleFrequency(oliverciphertext, trainingtext);
	cout << "decrypted: " << decryptedtext.substr(0, 20) << endl;

	// decrypt using pair frequencies
//	string decryptedtext = cipher->decryptUsingPairFrequency(decryptedtext, trainingtext);
//	cout << "decrypted: " << decryptedtext.substr(0, 20) << endl;


	// test
//	int cipherTextFreq[NUMLETT*NUMLETT][NUMLETT*NUMLETT];
//	cipher->countPairFrequency(oliverciphertext, cipherTextFreq);
//	ArrayUtil::outputArray("cipher   freq: " , cipherTextFreq[162], NUMLETT*NUMLETT);

}

void testPairSubstitutionCipher_CompareSingleFrequency_Anna_Wrnpc() {
	PairSubstitutionCipher *cipher = new PairSubstitutionCipher();

	//	int frequency[27*27][27*27];
	//	cipher->countFrequency("AAAB", frequency);
	//	cout << frequency[0][1] << endl;

	//	int singleFreq[27*27];
	//	cipher->countSinglePairFrequency("aaacbaba  ", singleFreq);
	//	cout << "aa: " << singleFreq[0] << endl;
	//	cout << ", ac: " << singleFreq[2] << endl;
	//	cout << ", ba: " << singleFreq[27] << endl;
	//	cout << ",   : " << singleFreq[27*27-1] << endl;

	// compare single frequencies of pairs of characters from war_n_peace and anna_karenina
	string trainingtext = TextFileUtil::readTextFromFile("wrnpc12.txt");
	trainingtext = TextFileUtil::getLatinAlphabetString(trainingtext);
	string trainingtext1 = TextFileUtil::readTextFromFile("anna.txt");
	trainingtext1 = TextFileUtil::getLatinAlphabetString(trainingtext1);
	int freq[26*26];
	int freq1[26*26];
	cipher->countSinglePairFrequency(trainingtext, freq);
	cipher->countSinglePairFrequency(trainingtext1, freq1);

	ArrayUtil::outputArray("wrnpc", freq, 26*26 );
	ArrayUtil::outputArray("annak", freq1, 26*26 );

	int order[26*26];
	int order1[26*26];
	SortUtil::getDescendingOrder(freq, order, 26*26);
	SortUtil::getDescendingOrder(freq1, order1, 26*26);
	ArrayUtil::outputArray("wrnpc order", order, 26*26);
	ArrayUtil::outputArray("annak order", order1, 26*26);

}

void testSubstitutionTranspositionCipher() {
	RandUtil* randUtil = new RandUtil();

	SubstitutionTranspositionCipher* stCipher = new SubstitutionTranspositionCipher();

	string plaintext = TextFileUtil::readTextFromFile("oliverorig");
	string trainingtext = TextFileUtil::readTextFromFile("warorig");


	int noOfRun = 100;
	int keylengthTransposisiton = 20;
	int numCycle = 3;
	int numIterationSubstitution  = 10*1000;
	int numIterationTransposition =  10*1000;
	int ciphertextlength = 2*1000;

	double correctness[noOfRun];
	cout << endl;
	for (int i=0; i<noOfRun; i++) {
		cout << endl << "Run " << i+1 << "/" << noOfRun << endl;

		// encryption
		int* key = randUtil->shuffle(keylengthTransposisiton);
		string oliverciphertext = stCipher->encrypt(plaintext, key, keylengthTransposisiton);
//		TextFileUtil::writeTextToFile("oliver.substitution.transposition.ciphertext", oliverciphertext);

//		string oliverciphertext = TextFileUtil::readTextFromFile("oliver.substitution.transposition.ciphertext");
		int startposition = randUtil->newIntRand(plaintext.length()-keylengthTransposisiton) / keylengthTransposisiton * keylengthTransposisiton;
		oliverciphertext = oliverciphertext.substr(startposition, ciphertextlength);

		// decryption
		double startTime = SysUtil::getCurrentTimestamp();
		string oliverdecryptedtext = stCipher->decrypt(oliverciphertext, trainingtext, keylengthTransposisiton, numCycle, numIterationSubstitution, numIterationTransposition);
		cout << "Elapsed time in seconds: " << setprecision(6) << SysUtil::calculateElapsedTime(startTime) << endl;

		cout << "start position: " << startposition << endl;
		cout << "decrypted text: " << oliverdecryptedtext.substr(0, 80) << endl;

		correctness[i] = stCipher->verify(plaintext.substr(startposition), oliverdecryptedtext, keylengthTransposisiton);

	}
	cout << endl;
	ArrayUtil::outputArray("Correctness: ", correctness, noOfRun);
	cout << "Average correctness: " << ArrayUtil::average(correctness, noOfRun) << endl;
	cout << "Average no of correct positions: " << (keylengthTransposisiton*ArrayUtil::average(correctness, noOfRun)) << endl;
	cout << "No of successful decryptions: " << ArrayUtil::countNoOfOccur(1.00, correctness, noOfRun) << endl;

}

int main() {
	//test();
	//testShuffle();
	//testTextFileUtil();
	//testSortUtil();
	//testConvUtil();
	//testSlideMove();
	//testSysUtil();


	//testSubstitutionCipher();
	//testSubstitutionCipher_DifferentPartOfCipherText();

	//testTranspositionCipher();
	//testTranspositionCipher_UnknownKeyLength();
	//testTranspositionCipher_DifferentPartOfCipherText();

	//testTranspositionCipher_repetition();

	//testPairSubstitutionCipher();
	//testPairSubstitutionCipher_CompareSingleFrequency_Anna_Wrnpc();

	testSubstitutionTranspositionCipher();



	return 0;
}
