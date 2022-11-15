#include <iostream>
#include <string>
#include <vector>

using namespace std;

vector<vector<int>> matrixMultiply(vector<int> m1, vector<int> m2);
int vectorMultiplyAlphas(vector<int> alphas, vector<int> X, int symbol);
int alphaAdd(int alpha1, int alpha2);
int alphaMultiply(int alpha1, int alpha2);

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
	cout << "Enter your 11 symbol message: " << endl;
	cout << "Note: the symbols will be processed in the same order you enter them\n";
	while (message.size() < 11) {
		int temp;
		cin >> temp;
		message.push_back(temp);
	}

	vector<int> registers = { 15, 15, 15, 15 };
	vector<int> codeword = {};
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
		codeword.push_back(registers[i]);
	}
	for (int i = 10; i >= 0; i--) {
		codeword.push_back(message[i]);
	}

	// Print out our 15 symbol code word to console
	cout << "Codeword U(x): " << endl;
	for (int n : codeword) {
		cout << n << " ";
	}
	cout << endl;

	// Validate U(x) for each of the 4 roots
	vector<int> X = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 };
	cout << "Testing alpha-1: " << vectorMultiplyAlphas(codeword, X, 1) << endl;
	cout << "Testing alpha-2: " << vectorMultiplyAlphas(codeword, X, 2) << endl;
	cout << "Testing alpha-3: " << vectorMultiplyAlphas(codeword, X, 3) << endl;
	cout << "Testing alpha-4: " << vectorMultiplyAlphas(codeword, X, 4) << endl << endl;

	cout << "";

	return 0;
}


vector<vector<int>> matrixMultiply(vector<int> m1, vector<int> m2)
{
	return vector<vector<int>>();
}

int vectorMultiplyAlphas(vector<int> alphas, vector<int> X, int symbol)
{
	int sum = 15;
	if (alphas.size() != X.size()) exit(1);
	for (int i = 0; i < alphas.size(); i++) {
		sum = alphaAdd(sum, (alphaMultiply(alphaMultiply(X[i], symbol), alphas[i])));
	}
	return sum;
}

int alphaAdd(int alpha1, int alpha2)
{
	return addTable[alpha1][alpha2];
}

int alphaMultiply(int alpha1, int alpha2)
{
	if (alpha1 == 15) return 15;
	else if (alpha2 == 15) return 15;
	else return ((alpha1 + alpha2) % 15);
}
