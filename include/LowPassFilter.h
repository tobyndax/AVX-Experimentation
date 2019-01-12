#pragma once
#include "pgm.h"
#include <vector>

class LowPassFilter
{
public:
	LowPassFilter(const size_t symmetricSize);
	~LowPassFilter();
	SimpleImage apply(const SimpleImage& img);
private:
	const size_t kernelWidth = 0;
	const size_t kernelOffset = 0;
	size_t calculateIndexClamp(const int i, const  int j, const int k, const ImageInfo&  info);
};
