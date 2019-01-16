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

void evictCache() {
	const int size = 4 * 1024 * 1024; // Allocate 4M. Set much larger then L2 (0.25 MB/core for my i7-6600U
	char *c = (char *)malloc(size);
	for (int i = 0; i < 0x0fff; i++)
		for (int j = 0; j < size; j++)
			c[j] = i * j;
}

void runWithUninitializedArrays() {
	const int numElements = (int)std::pow(2, 26);
	float* data = new float[numElements]; //Create a lot of data.
	alignas(64) float* dataSIMD = new float[numElements]; //Create a lot of data.
	Time t1 = timeNow();

	for (int i = 0; i < numElements; i++) {
		data[i] = data[i] * data[i] + data[i] * 2; //d = d^2 + d;
	}
	std::cout << "Regular processing: " << durationMilli(timeNow() - t1) << "ms" << std::endl;

	t1 = timeNow();
	for (int i = 0; i < numElements / 8; i++) {
		__m256 __mdata = _mm256_loadu_ps(dataSIMD + i * 8); //(256)bit register (loadu)naligned of (p)recision (s)ingle
		__m256 temp = _mm256_mul_ps(__mdata, __mdata); //(256)bit register (mul)tiply (p)recision (s)ingle
		temp = _mm256_add_ps(temp, __mdata);
		_mm256_storeu_ps(dataSIMD + i * 8, temp);
	}
	std::cout << "SIMD processing: " << durationMilli(timeNow() - t1) << "ms" << std::endl;
}

void runWithInitializedArrays() {
	const int numElements = (int)(int)std::pow(2, 26);
	float* data = new float[numElements]; //Create a lot of data.
	alignas(64) float* dataSIMD = new float[numElements]; //Create a lot of data.
	std::generate_n(data, numElements, increment());
	std::generate_n(dataSIMD, numElements, increment());
	Time t1 = timeNow();
	for (int i = 0; i < numElements; i++) {
		data[i] = data[i] * data[i] + data[i] * 2; //d = d^2 + d;
	}
	std::cout << "Regular processing: " << durationMilli(timeNow() - t1) << "ms" << std::endl;

	t1 = timeNow();
	for (int i = 0; i < numElements / 8; i++) {
		__m256 __mdata = _mm256_loadu_ps(dataSIMD + i * 8); //(256)bit register (loadu)naligned of (p)recision (s)ingle
		__m256 temp = _mm256_mul_ps(__mdata, __mdata); //(256)bit register (mul)tiply (p)recision (s)ingle
		temp = _mm256_add_ps(temp, __mdata);
		_mm256_storeu_ps(dataSIMD + i * 8, temp);
	}
	std::cout << "SIMD processing: " << durationMilli(timeNow() - t1) << "ms" << std::endl;

}

void runDisableAutoVectorization() {
	const int numElements = (int)std::pow(2, 26);
	float* data = new float[numElements]; //Create a lot of data.
	alignas(64) float* dataSIMD = new float[numElements]; //Create a lot of data.
	std::generate_n(data, numElements, increment());
	std::generate_n(dataSIMD, numElements, increment());
	Time t1 = timeNow();
#pragma loop(no_vector) //without this, our compiler is likely emitting SSE2 instructions (128 wide registers).
	for (int i = 0; i < numElements; i++) { //Interestingly, in the dissassembly there seems to be XMM registers in use. But the loop is def. slower. 
		data[i] = data[i] * data[i] + data[i] * 2; //d = d^2 + d;
	}
	std::cout << "Regular processing: " << durationMilli(timeNow() - t1) << "ms" << std::endl;

	t1 = timeNow();
	for (int i = 0; i < numElements / 8; i++) {
		__m256 __mdata = _mm256_loadu_ps(dataSIMD + i * 8); //(256)bit register (loadu)naligned of (p)recision (s)ingle
		__m256 temp = _mm256_mul_ps(__mdata, __mdata); //(256)bit register (mul)tiply (p)recision (s)ingle
		temp = _mm256_add_ps(temp, __mdata);
		_mm256_storeu_ps(dataSIMD + i * 8, temp);
	}
	std::cout << "SIMD processing: " << durationMilli(timeNow() - t1) << "ms" << std::endl;
}


void runExampleOne() {
	std::cout
		<< "The example demonstrates the usage of instrinsics for AVX (256 bit registers)" << std::endl;
	std::cout
		<< "Here we compare the speed of a 'for-loop' version and AVX-intrinsics version: " << std::endl;
	pressToContinue();
	runWithInitializedArrays();

	std::cout
		<< "One would expect roughly a 6-8 times speed up between the naive loop version and the AVX-version." << std::endl
		<< "However, this is not the case. The speed-up should be closer to 2." << std::endl
		<< "This is because our current optimization settings allow for auto-vectorization (to 128 bit registers [XMM])" << std::endl
		<< "So our speed-up with our 256 bit registers should be close to a factor 2 instead." << std::endl << std::endl;


	std::cout
		<< "In the next run, I've disabled the auto-vectorization using #pragma loop(no_vector), to demonstrate." << std::endl;
	pressToContinue();
	evictCache();
	runDisableAutoVectorization();
	std::cout
		<< "As we can see, the speed difference is now much more significant" << std::endl
		<< "Looking into the dissassembly, you will find that the non-vectorized functions are also using the XMM registers. " << std::endl << std::endl;

	std::cout
		<< "Run with the arrays uninitialized. In this case you might see quite bad performance." << std::endl
		<< "Certain instructions are slower when encountering #INF or NaN. For instance add." << std::endl
		<< "With uninitialized data in the arrays, we might encounter this, we could also be 'lucky' and not encounter it :)." << std::endl << std::endl;
	pressToContinue();
	evictCache();
	runWithUninitializedArrays();

}
