#pragma once
#include <iostream>
#include <string>
#include <vector>

using namespace std;
// Function prototyping, these lines need to be here so the functions can call other functions in this file
vector<vector<int>> inverseMatrix(vector<vector<int>> m);
vector<int> findBetas(int sig1, int sig2);
void printMatrix(vector<vector<int>> m);
int vectorSumAlphas(vector<int> alphas, vector<int> X, int symbol);
int alphaAdd(int alpha1, int alpha2);
int alphaMultiply(int alpha1, int alpha2);
int alphaExponent(int alpha, int exponent);

/* 
* This inverseMatrix function takes in a 2x2 matrix and returns the inverse.
* @param m - a 2x2 matrix
* @return inverse - the inverse of m
*/
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

/*
* The find beta functions takes a given Sigma1 and Sigma2 value to test the equation Sigma(x) = a0 + Sig1X + Sig2X2 and find the error value locations.
* @param sig1 - the alpha sigma 1 value
* @param sig2 - the alpha sigma 2 value
* @return betas - a size-2 vector that contains both error locations
*/
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
	if (betas[0] != 0) {
		betas[0] *= -1;
		betas[0] += 15;
	}
	betas[1] *= -1;
	betas[1] += 15;

	return betas;
}

/* This is a helper/debugger function used to print out a 2x2 matrix into the console for viewing/confirming.
*/
void printMatrix(vector<vector<int>> m)
{
	cout << m[0][0] << " " << m[0][1] << endl;
	cout << m[1][0] << " " << m[1][1] << endl;
}

/* This is a helper function that helps us multiply all alpha values in a vector by a given root value. We
* can use this to calculate our Roots for U(X) and Syndrome values quickly.
* @param alphas - original vector of alpha values, like U(x) or R(x)
* @param X - constant vector of X values (X0, X1, X2, ... , X14)
* @param symbol - The value of the parameter we are inputting for x in U(x) or R(x), 1 for alpha1, 2 for alpha2, ... etc.
* @return sum - The final sum value of U(x) or R(x) given a specific x input.
*/
int vectorSumAlphas(vector<int> alphas, vector<int> X, int symbol)
{
	int sum = 15;
	for (int i = 0; i < alphas.size(); i++) {
		sum = alphaAdd(sum, alphaMultiply(alphas[i], (alphaExponent(symbol, X[i]))));
	}
	return sum;
}

// Alpha addition table, 0 is alpha0 and 15 is true 0. The other numbers represent themselves.
// EG 10 = alpha10, 5 = alpha5. It is read by using as alpha1 + alpha2 = [alpha1][alpha2] = [row][column].
vector<vector<int>> addTable =
{  {15, 4,  8,  14, 1,  10, 13, 9,  2,  7,  5,  12, 11, 6,  3,  0},
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

/* This is our helper function that helps us resolve alpha-addition by using the above hard-coded addition table.
* If we have a7 and a6 and want to add them together (a7 + a6), we can find the result by accessing row 7 column 6 or
* row 6 column 7 and return it.
* @param alpha1 - the first alpha to be added
* @param alpha2 - the second alpha to be added
* return - the new value after processing the sum of alpha1 and alpha2
*/
int alphaAdd(int alpha1, int alpha2) {
	return addTable[alpha1][alpha2];
}

/* This is our helper function that helps us computer alpha-symbol multiplication.
EG (a7 * a6 = a13; a9 * 14 = (a23 % a15) = a8
*/
int alphaMultiply(int alpha1, int alpha2) {
	if (alpha1 == 15) return 15;
	else if (alpha2 == 15) return 15;
	else return ((alpha1 + alpha2) % 15);
}

/* Helper function that helps resolve substituting an X position with an alpha value. EG plugging in alpha2 into
* X4 will yield alpha8. Completes the exponent multiplication for alpha values.
* @param alpha - the alpha value being plugged into X position
* @param X - the X position or the exponent value
* @return - the new alpha value after adjusting the exponent.
*/
int alphaExponent(int alpha, int exponent) {
	if (alpha == 0) return 15;
	else if (exponent == 0) return 0;
	else return (alpha * exponent) % 15;
}
