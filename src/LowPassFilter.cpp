#include "LowPassFilter.h"


LowPassFilter::LowPassFilter(const size_t symmetricSize) : kernelWidth(symmetricSize), kernelOffset(std::floor(symmetricSize / 2))
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
	SimpleImage ping{ info };
	std::vector<uint8_t>& pingData = ping.getData();// We write to this buffer first, and then in pass two, write to output. 
	std::vector<uint8_t>& outputData = output.getData();// We write to this buffer first, and then in pass two, write to output. 

	for (int j = 0; j < info.height; j++) {
		for (int i = 0; i < info.width; i++) {
			float localSum = 0;
			for (size_t k = 0; k < kernelWidth; k++) {
				size_t index = calculateIndexClamp(i, j, k, info);
				localSum += data[index];
			}
			localSum /= kernelWidth;
			pingData[i + j * info.width] = static_cast<uint8_t>(localSum); // write back 
		}
	}

	for (int j = 0; j < info.height; j++) {
		for (int i = 0; i < info.width; i++) {
			float localSum = 0;
			for (size_t k = 0; k < kernelWidth; k++) {
				size_t index = calculateIndexClamp_transposed(i, j, k, info);
				localSum += pingData[index];
			}
			localSum /= kernelWidth;
			outputData[j* info.width + i] = static_cast<uint8_t>(localSum); // write back 
		}
	}


	return output;
}



size_t LowPassFilter::calculateIndexClamp_transposed(const int i, const int j, const int k, const ImageInfo & info)
{
	int index = j+k; //figure out which row we're addressing
	index -= kernelOffset;

	index = index < 0 ? 0 : index; // clamp out of bounds
	index = index > info.height - 1 ? info.height - 1 : index; // clamp out of bounds
	index = index * info.width; // offset into data
	index += i; // offset to correct row

	return index;
}



size_t LowPassFilter::calculateIndexClamp(const int i, const int j, const int k, const ImageInfo & info)
{
	int index = i + k; //figure out which column we're addressing
	
	index -= kernelOffset;
	index = index < 0 ? 0 : index; // if left of column 0, clamp to zero.
	index = index > info.width -1  ? info.width -1 : index; //if right of column width-1, clamp to width-1.
	index += info.width*j; // offset to row j.

	return index;
}


