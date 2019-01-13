/* Author Jens Edhammer
*/
#include "pgm.h"
#include "LowPassFilter.h"
#include "TimeUtils.h"


#include <iostream>
#include <algorithm>
#include <string>
#include <immintrin.h>



/*  This small code base contains a PlainPGM (ASCII "P2") reader and writer to easily get images into the program
 *  and to easily output them to file again. 
 *  The idea with the code is to facilitate easy experimentation with SIMD optimization techniques
 *
 *	The image load and save functions are to help with experimenting with image processing algos and SIMD.
 */

struct increment {
	int value;
	int operator() () { return ++value; }
	increment() :value(0) {}
};

void runWithUninitializedArrays() {
	const int numElements = std::pow(2, 30);
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
	const int numElements = std::pow(2, 30);
	float* data = new float[numElements]; //Create a lot of data.
	alignas(64) float* dataSIMD = new float[numElements]; //Create a lot of data.
	std::generate_n(data,numElements, increment());
	std::generate_n(dataSIMD , numElements, increment());
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

	const int numElements = std::pow(2, 30);
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

//Create
int NotMain(){
    PGMReader pgmReader;
    SimpleImage* img = pgmReader.read("H:/Programmering/AVX-Experimentation/testData/baboon.ascii.pgm");
	std::cout << "Width: " << img->getImageInfo().width << " Height: " << img->getImageInfo().height << std::endl;

    //Process the image here. 
	LowPassFilter filt{ 9 };
	SimpleImage outputImg{0,0};

	Time t1 = timeNow();
	const int iterations = 10;
	for (int i = 0; i < iterations; i++) {
		outputImg = filt.apply(*img);
	}
	
	std::cout << durationMilli(timeNow() - t1) / iterations << "ms" << std::endl;

    PGMWriter writer;
    writer.write("H:/Programmering/AVX-Experimentation/baboon_mine.ascii.pgm", outputImg);

    return 0;
}

//Create some float data and process it with raw intrinsics.
int main() {
	/*std::cout << "UninitializedArrays" << std::endl;
	runWithUninitializedArrays();
	std::cout << "InitializedArrays" << std::endl;
	runWithInitializedArrays();*/

	std::cout << "No AutoVectorization" << std::endl;
	runDisableAutoVectorization();


	std::cin.ignore();
	return 0;
}
