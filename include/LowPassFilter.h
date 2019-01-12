#pragma once
#include "pgm.h"
#include <vector>

class Kernel
{
public:
	Kernel() : size(0), kernelOffset(0) {};
	Kernel(const size_t size, const float* values);
	~Kernel();

	float* _kernel = nullptr;

	const size_t size;
	const size_t kernelOffset;
};

class LowPassFilter
{
public:
	LowPassFilter(const size_t symmetricSize);
	~LowPassFilter();
	SimpleImage apply(const SimpleImage& img);
private:
	size_t kernelWidth = 0;

	size_t calculateIndexClamp(const int i, const  int j, const int k, const ImageInfo&  info);
};
