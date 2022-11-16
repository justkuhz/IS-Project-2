#include <iostream>
#include <string>
#include <vector>

using namespace std;

int vectorMultiplyAlphas(vector<int> alphas, vector<int> X, int symbol);
int alphaAdd(int alpha1, int alpha2);
int alphaMultiply(int alpha1, int alpha2);
int alphaExponent(int alpha, int exponent);
vector<vector<int>> inverseMatrix(vector<vector<int>> m);
vector<int> findBetas(int sig1, int sig2);
void printMatrix(vector<vector<int>> m);

vector<vector<int>> addTable =
{   {15, 4,  8,  14, 1,  10, 13, 9,  2,  7,  5,  12, 11, 6,  3,  0},
	{4,  15, 5,  9,  0,  2,  11, 14, 10, 3,  8,  6,  13, 12, 7,  1},
	{8,  5,  15, 6,  10, 1,  3,  12, 0,  11, 4,  9,  7,  14, 13, 2},
	{14, 9,  6 , 15, 7,  11, 2,  4,  13, 1,  12, 5,  10, 8,  0,  3},
	{1,  0,  10, 7,  15, 8,  12, 3,  5,  14, 2,  13, 6,  11, 9,  4},
	{10, 2,  1,  11, 8,  15, 9,  13, 4,  6,  0,  3,  14, 7,  12, 5},
	{13, 11, 3,  2,  12, 9,  15, 10, 14, 5,  7,  1,  4,  0,  8,  6},
	{9,  14, 12, 4,  3,  13, 10, 15, 11, 0,  6,  8,  2,  5,  1,  7},
	{2,  10, 0,  13, 5,  4,  14, 11, 15, 12, 1,  7,  9,  3,  6,  8},
	{7,  3,  11, 1,  14, 6,  5,  0,  12, 15, 13, 2,  8,  10, 4,  9},
	{5,  8,  4,  12, 2,  0,  7,  6,  1,  13, 15, 14, 3,  9,  11, 10},
	{12, 6,  9,  5,  13, 3,  1,  8,  7,  2,  14, 15, 0,  4,  10, 11},
	{11, 13, 7,  10, 6,  14, 4,  2,  9,  8,  3,  0,  15, 1,  5,  12},
	{6,  12, 14, 8,  11, 7,  0,  5,  3,  10, 9,  4,  1,  15, 2,  13},
	{3,  7,  13, 0,  9,  12, 8,  1,  6,  4,  11, 10, 5,  2,  15, 14},
	{0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14, 15}
};

int main() {
	vector<int> message = {};
	cout << "Enter your 11 symbol message RS(15, 11): " << endl;
	cout << "The symbols should be between 0 which represents a0 and 15 which is true 0\n";
	cout << "Note: the symbols will be processed in the same order you enter them\n";
	while (message.size() < 11) {
		int temp;
		do {
			cin >> temp;
			if (temp < 0 || temp > 15) {
				cout << "That is not a valid input, try again.\n";
			}
		} while (temp < 0 || temp > 15);
		message.push_back(temp);
	}

	vector<int> registers = { 15, 15, 15, 15 };
	vector<int> codeWord = {};
	int feedback = 15;
	// State Machine Cycles
	for (int i = 0; i < message.size(); i++) {
		
		feedback = alphaAdd(message[i], registers[3]);
		registers[3] = alphaAdd(alphaMultiply(feedback, 13), registers[2]);
		registers[2] = alphaAdd(alphaMultiply(feedback, 6), registers[1]);
		registers[1] = alphaAdd(alphaMultiply(feedback, 3), registers[0]);
		registers[0] = alphaMultiply(feedback, 10);

		/*cout << "Cycle " << i + 1 << ": " << endl;
		cout << "R1: " << registers[0] << " R2: " << registers[1] << " R3: " << registers[2] <<
			" R4: " << registers[3] << " Feedback: " << feedback << endl;
			*/
	}
	cout << "Registers: " << endl;
	cout << "R1: " << registers[0] << " R2: " << registers[1] << " R3: " << registers[2] <<
		" R4: " << registers[3] << endl << endl;

	// Push alpha values into correct positions in code word
	for (int i = 0; i < 4; i++) {
		codeWord.push_back(registers[i]);
	}
	for (int i = 10; i >= 0; i--) {
		codeWord.push_back(message[i]);
	}

	// Print out our 15 symbol code word to console
	cout << "Codeword U(x): " << endl;
	for (int i = 0; i < codeWord.size(); i++) {
		if (i == codeWord.size() - 1) cout << codeWord[i];
		else cout << codeWord[i] << " + ";
	}
	cout << endl;

	// Validate U(x) for each of the 4 roots
	vector<int> X = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 };

	// Debugging print statements
	// cout << "Testing alpha-1: " << vectorMultiplyAlphas(codeword, X, 1) << endl;
	// cout << "Testing alpha-2: " << vectorMultiplyAlphas(codeword, X, 2) << endl;
	// cout << "Testing alpha-3: " << vectorMultiplyAlphas(codeword, X, 3) << endl;
	// cout << "Testing alpha-4: " << vectorMultiplyAlphas(codeword, X, 4) << endl << endl;

	int check1 = vectorMultiplyAlphas(codeWord, X, 1);
	int check2 = vectorMultiplyAlphas(codeWord, X, 2);
	int check3 = vectorMultiplyAlphas(codeWord, X, 3);
	int check4 = vectorMultiplyAlphas(codeWord, X, 4);
	if (check1 == 15 && check2 == 15 && check3 == 15 && check4 == 15) {
		cout << "All Clear! You have a valid codeword U(x).\n" << endl;
	}
	else {
		return 1;
	}

	// Enter the location and symbols of error message e(x)
	int errorX1, errorX2, errorA1, errorA2;
	
	do {
		cout << "Enter the X-location of error symbol 1 (between 0 and 14): ";
		cin >> errorX1;
	} while (errorX1 < 0 || errorX1 > 14);

	do {
		cout << "What error symbol to use (between 0 and 14): ";
		cin >> errorA1;
	} while (errorA1 < 0 || errorA1 > 14);


	switch (errorX1) {
	case 0: 
		errorX2 = 1;
		cout << "The X-location of error symbol 2 is set to X1 by default.\n"; 
		break;

	case 14: 
		errorX2 = 13;
		cout << "The X-location of error symbol 2 is set to X13 by default.\n"; 
		break;

	default: 
		do {
			cout << "Select either " << errorX1 - 1 << " or " << errorX1 + 1
				<< " to be the X-location of error symbol 2: ";
			cin >> errorX2;
	} while (errorX2 != errorX1 - 1 && errorX2 != errorX1 + 1);
	break;
	}

	do {
		cout << "What error symbol to use (between 0 and 14): ";
		cin >> errorA2;
	} while (errorA2 < 0 || errorA2 > 14);

	// creation of R(x) which is U(x) + e(x)
	vector<int> errorCode = codeWord;
	errorCode[errorX1] = alphaAdd(errorA1, codeWord[errorX1]);
	errorCode[errorX2] = alphaAdd(errorA2, codeWord[errorX2]);

	// Printing R(x)
	cout << "R(x): " << endl;
	for (int i = 0; i < errorCode.size(); i++) {
		if (i == errorCode.size() - 1) cout << errorCode[i];
		else cout << errorCode[i] << " + ";
	}
	cout << endl;

	// Calculation of error syndromes 1 through 4 using R(x)
	int syn1, syn2, syn3, syn4;
	syn1 = vectorMultiplyAlphas(errorCode, X, 1);
	syn2 = vectorMultiplyAlphas(errorCode, X, 2);
	syn3 = vectorMultiplyAlphas(errorCode, X, 3);
	syn4 = vectorMultiplyAlphas(errorCode, X, 4);

	// Debug and print out our syndrome values
	cout << "\nSyndrome values:\n";
	cout << "S1 = " << syn1 << endl;
	cout << "S2 = " << syn2 << endl;
	cout << "S3 = " << syn3 << endl;
	cout << "S4 = " << syn4 << endl << endl;

	// Use syndromes to find location of errors
	vector<vector<int>> matrix = { {syn1, syn2},
		                           {syn2, syn3} };
	
	// Inverse matrix to use for Sigma1 Sigma2 equation
	vector<vector<int>> inverse = inverseMatrix(matrix);

	//cout << "Inverse Matrix: " << endl;
	//printMatrix(inverse);

	int sig1 = alphaAdd(alphaMultiply(inverse[1][0], syn3), alphaMultiply(inverse[1][1], syn4));
	int sig2 = alphaAdd(alphaMultiply(inverse[0][0], syn3), alphaMultiply(inverse[0][1], syn4));

	cout << "Sig1: " << sig1 << " Sig2: " << sig2 << endl << endl;

	vector<int> betas = findBetas(sig1, sig2);

	if (betas[0] != 0) {
		betas[0] *= -1;
		betas[0] += 15;
	}
	betas[1] *= -1;
	betas[1] += 15;

	// Exit program if both error locations are in parity bits
	if (betas[0] < 4 && betas[1] < 4) {
		cout << "Both errors occurred in parity bits, we do not care.";
		return 0;
	}

	// Print Error Locations
	cout << "Error Locations: " << endl;
	cout << "location 1: a" << betas[0] << " location 2: a" << betas[1] << endl;

	// Solving for error values to replace in the locations
	vector<vector<int>> M = { {betas[0], betas[1]},
		                      {alphaExponent(betas[0], 2), alphaExponent(betas[1], 2)}};

	vector<vector<int>> invM = inverseMatrix(M);

	int error1 = alphaAdd(alphaMultiply(invM[0][0], syn1), alphaMultiply(invM[0][1], syn2));
	int error2 = alphaAdd(alphaMultiply(invM[1][0], syn1), alphaMultiply(invM[1][1], syn2));
	cout << "\nError Values:\n" << "a" << error1 << "X" << betas[0] << endl;
	cout << "a" << error2 << "X" << betas[1] << endl;

	// Add error values back into R(x) to correct R(x) back into U(x)
	cout << "R(x) before correction: " << endl;
	for (int i = 0; i < errorCode.size(); i++) {
		if (i == errorCode.size() - 1) cout << errorCode[i];
		else cout << errorCode[i] << " + ";
	}
	cout << endl << endl;

	cout << "Adding in e'(x) into R(x) to revert to U(x):" << endl;
	cout << "New R(x) ( == U(x)):" << endl;
	
	errorCode[betas[0]] = alphaAdd(error1, errorCode[betas[0]]);
	errorCode[betas[1]] = alphaAdd(error2, errorCode[betas[1]]);

	for (int i = 0; i < errorCode.size(); i++) {
		if (i == errorCode.size() - 1) cout << errorCode[i];
		else cout << errorCode[i] << " + ";
	}
	cout << endl << endl;

	cout << "Compare to original codeword message U(x): " << endl;
	for (int i = 0; i < codeWord.size(); i++) {
		if (i == codeWord.size() - 1) cout << codeWord[i];
		else cout << codeWord[i] << " + ";
	}
	cout << endl;

	return 0;
}

vector<vector<int>> inverseMatrix(vector<vector<int>> m) {
	//cout << "Matrix: " << endl;
	//printMatrix(m);

	int determinant = alphaAdd(alphaMultiply(m[0][0], m[1][1]), alphaMultiply(m[0][1], m[1][0]));

	//cout << "Determinant: " << determinant << endl;

	vector<vector<int>> cofactor = { {m[1][1], m[0][1]},
		                             {m[1][0], m[0][0]} };

	//cout << "Cofactor: " << endl;
	//printMatrix(cofactor);

	vector<vector<int>> inverse = { {15, 15},
		                            {15, 15} };
	determinant *= -1;
	if (determinant < 0) determinant += 15;

	inverse[0][0] = alphaMultiply(cofactor[0][0], determinant);
	inverse[0][1] = alphaMultiply(cofactor[0][1], determinant);
	inverse[1][0] = alphaMultiply(cofactor[1][0], determinant);
	inverse[1][1] = alphaMultiply(cofactor[1][1], determinant);

	return inverse;
}

vector<int> findBetas(int sig1, int sig2) {

	vector<int> alphas = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 };
	vector<int> betas = {};

	for (int i = 0; i < alphas.size(); i++) {
		int x = 0, y = 0, z = 0;
		// set y to sig1x1
		y = alphaMultiply(sig1, alphaExponent(alphas[i], 1));
		// set z to sig2x2
		z = alphaMultiply(sig2, alphaExponent(alphas[i], 2));

		// Debugging print statements
		// cout << "Sig(a" << alphas[i] << "): ";
		// cout << alphaAdd(alphaAdd(x, y), z) << endl;

		if (alphaAdd(alphaAdd(x, y), z) == 15) {
			betas.push_back(alphas[i]);
			if (betas.size() == 2) break;
		}
	}

	return betas;
}

void printMatrix(vector<vector<int>> m)
{
	cout << m[0][0] << " " << m[0][1] << endl;
	cout << m[1][0] << " " << m[1][1] << endl;
}


int vectorMultiplyAlphas(vector<int> alphas, vector<int> X, int symbol)
{
	int sum = 15;
	for (int i = 0; i < alphas.size(); i++) {
		sum = alphaAdd(sum, alphaMultiply(alphas[i], (alphaExponent(symbol, X[i]))));
	}
	return sum;
}

int alphaAdd(int alpha1, int alpha2) {
	return addTable[alpha1][alpha2];
}

int alphaMultiply(int alpha1, int alpha2) {
	if (alpha1 == 15) return 15;
	else if (alpha2 == 15) return 15;
	else return ((alpha1 + alpha2) % 15);
}

int alphaExponent(int alpha, int exponent) {
	if (alpha == 0) return 15;
	else if (exponent == 0) return 0;
	else return (alpha * exponent) % 15;
}
