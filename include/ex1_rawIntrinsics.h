#pragma once
#include "TimeUtils.h"
#include <iostream>
#include <algorithm>
#include <immintrin.h>


struct increment {
	int value;
	int operator() () { return ++value; }
	increment() :value(0) {}
};

void pressToContinue() {
	std::cout << "Press any key to continue..." << std::endl;
	std::cin.ignore();
}

void runWithInitializedArrays(const int elementPower,const int iterations = 1) {
	const int numElements = (int)(int)std::pow(2, elementPower);
	float* data = new float[numElements]; //Create a lot of data.
	alignas(64) float* dataSIMD = new float[numElements]; //Create a lot of data.
	std::generate_n(data, numElements, increment());
	std::generate_n(dataSIMD, numElements, increment());
	Time t1 = timeNow();
	for (int its = 0; its < iterations; its++) {
		for (int i = 0; i < numElements / 8; i++) {
			__m256 __mdata = _mm256_loadu_ps(dataSIMD + i * 8); //(256)bit register (loadu)naligned of (p)recision (s)ingle
			__m256 temp = _mm256_mul_ps(__mdata, __mdata); //(256)bit register (mul)tiply (p)recision (s)ingle
			temp = _mm256_add_ps(temp, __mdata);
			_mm256_storeu_ps(dataSIMD + i * 8, temp);
		}
	}
	std::cout << "SIMD processing: " << durationMilli(timeNow() - t1) / iterations << "ms" << std::endl;
	t1 = timeNow();
	for (int its = 0; its < iterations; its++) {
		for (int i = 0; i < numElements; i++) {
			data[i] = data[i] * data[i] + data[i] * 2; //d = d^2 + d;
		}
	}
	std::cout << "Regular processing: " << durationMilli(timeNow() - t1) / iterations << "ms" << std::endl;
	delete data;
	delete dataSIMD; 

}

void runExampleOne() {
	std::cout
		<< "The example demonstrates the usage of instrinsics for AVX (256 bit registers)" << std::endl;
	std::cout
		<< "Here we compare the speed of a 'for-loop' version and AVX-intrinsics version: " << std::endl;
	pressToContinue();
	runWithInitializedArrays(24,10);

	std::cout
		<< "One would expect roughly a 6-8 times speed up between the naive loop version and the AVX-version." << std::endl
		<< "However, this is not the case. The speed-up should be closer to 2." << std::endl
		<< "This is because our current optimization settings allow for auto-vectorization (to 128 bit registers [XMM])" << std::endl
		<< "So our speed-up with our 256 bit registers should be close to a factor 2 instead." << std::endl << std::endl;

	std::cout
		<< "One can verify this by examining the dissassembly of the loops. When XMM registers are present, "
		<< "one can suspect that SSE2 vectorization is in use" << std::endl << std::endl;

	pressToContinue();
	pressToContinue();
}
