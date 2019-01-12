#include "LowPassFilter.h"

Kernel::Kernel(const size_t size, const float * values) :size(size), kernelOffset(std::floor(size / 2))
{
	_kernel = new float[size];
	memcpy_s(_kernel, size, values, size);
}

Kernel::~Kernel()
{
	delete _kernel;
}

LowPassFilter::LowPassFilter(const size_t symmetricSize) : kernelWidth(symmetricSize)
{
}
LowPassFilter::~LowPassFilter()
{
}

SimpleImage LowPassFilter::apply(const SimpleImage & img)
{
	const ImageInfo info = img.getImageInfo();
	std::vector<uint8_t> data = img.getData();
	SimpleImage output{ info };
	SimpleImage ping{ info }; // We write to this buffer first, and then in pass two, write to output. 
	for (int j = 0; j < info.height; j++) {
		for (int i = 0; i < info.width; i++) {
			float localSum = 0;
			for (size_t k = 0; k < kernelWidth; k++) {
				size_t index = calculateIndexClamp(i, j, k, info);
				localSum += data[index];
			}
			localSum /= kernelWidth;
		}
	}

	return output;
}

size_t LowPassFilter::calculateIndexClamp(const int i, const int j, const int k, const ImageInfo & info)
{
	int index = i + info.width*j + k;
	index -= kernel->kernelOffset;

	return index;
}


