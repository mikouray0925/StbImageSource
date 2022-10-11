#pragma once
using namespace std;

namespace MyMath {
	int GetNumberOfCombinations(int numOfSamples, int quantityToTake) {
		if (quantityToTake > numOfSamples) {
			return 0;
		}
		if (quantityToTake == 0) {
			return 1;
		}
		if (quantityToTake > (numOfSamples - quantityToTake)) {
			quantityToTake = numOfSamples - quantityToTake;
		}

		int numerator = 1;
		for (int i = numOfSamples - quantityToTake + 1; i <= numOfSamples; i++) {
			numerator *= i;
		}

		int denominator = 1;
		for (int i = 1; i <= quantityToTake; i++) {
			denominator *= i;
		}

		return numerator / denominator;
	}

	void GenerateSequenceFromBaskaTriangle(int* buffer, int size) {
		if (size <= 0) 
			return;

		for (int i = 0; i <= size / 2; i++) {
			int num = GetNumberOfCombinations(size - 1, i);
			buffer[i] = num;
			buffer[size - 1 - i] = num;
		}
	}

	double GenerateGaussianMatrix(double* buffer, int size, double multiplier = 1) {
		int* seq = new int[size];
		GenerateSequenceFromBaskaTriangle(seq, size);

		double sum = 0;
		for (int i = 0; i < size; i++) {
			for (int j = 0; j < size; j++) {
				double val = double(seq[i] * seq[j]) * multiplier;
				buffer[i * size + j] = val;
				sum += val;
			}
		}

		delete[] seq;
		return sum;
	}

	double GenerateBartlettMatrix(double* buffer, int size, double multiplier = 1) {
		int* seq = new int[size];
		for (int i = 0; i <= size / 2; i++) {
			seq[i] = i + 1;
			seq[size - i - 1] = i + 1;
		}

		double sum = 0;
		for (int i = 0; i < size; i++) {
			for (int j = 0; j < size; j++) {
				double val = double(seq[i] * seq[j]) * multiplier;
				buffer[i * size + j] = val;
				sum += val;
			}
		}

		delete[] seq;
		return sum;
	}
};
