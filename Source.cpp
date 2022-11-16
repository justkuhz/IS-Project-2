#include "Header.h"

int main() {
	vector<int> message = {};

	// Ask for 11 message bit user input

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

	// Run through machine cycles to find additional 4 parity bits
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

	// Push alpha values into correct positions in code word U(x)
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

	int check1 = vectorSumAlphas(codeWord, X, 1);
	int check2 = vectorSumAlphas(codeWord, X, 2);
	int check3 = vectorSumAlphas(codeWord, X, 3);
	int check4 = vectorSumAlphas(codeWord, X, 4);
	if (check1 == 15 && check2 == 15 && check3 == 15 && check4 == 15) {
		cout << "All Clear! You have a valid codeword U(x).\n" << endl;
	}
	else {
		// Exit application if codeword is invalid/fails any root checks
		return 1;
	}

	// Get user input for the location and symbols of error message e(x)
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
	syn1 = vectorSumAlphas(errorCode, X, 1);
	syn2 = vectorSumAlphas(errorCode, X, 2);
	syn3 = vectorSumAlphas(errorCode, X, 3);
	syn4 = vectorSumAlphas(errorCode, X, 4);

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

	// Calculate and debug sigma values using syndrome values and inverse matrix
	int sig1 = alphaAdd(alphaMultiply(inverse[1][0], syn3), alphaMultiply(inverse[1][1], syn4));
	int sig2 = alphaAdd(alphaMultiply(inverse[0][0], syn3), alphaMultiply(inverse[0][1], syn4));

	cout << "Sig1: " << sig1 << " Sig2: " << sig2 << endl << endl;


	// Sigma values are passed into findBetas function to calculate beta values
	vector<int> betas = findBetas(sig1, sig2);

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

	// Printing error valyes
	cout << "\nError Values:\n" << "a" << error1 << "X" << betas[0] << endl;
	cout << "a" << error2 << "X" << betas[1] << endl;

	// Add error values back into R(x) to correct R(x) back into U(x)
	cout << "\nR(x) before correction: " << endl;
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