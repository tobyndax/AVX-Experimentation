/* Author Jens Edhammer
*/
#include "pgm.h"
#include "LowPassFilter.h"
#include "TimeUtils.h"


#include <iostream>
#include <string>
#include <immintrin.h>

/*  This small code base contains a PlainPGM (ASCII "P2") reader and writer to easily get images into the program
 *  and to easily output them to file again. 
 *  The idea with the code is to facilitate easy experimentation with SIMD optimization techniques
 *  specifically for image processing. (for example simple pixel arithmetic or lowpass filtering?)
 * 
 *  Read Image -> Process -> Save Image 
 * 
 *  Hopefully in the future this will also include some simple timing functionality to check the 
 *  performance of the functions. 
 */

int main(){
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
	
	std::cout << durationMilli(timeNow() - t1) / iterations << " ms" << std::endl;

    PGMWriter writer;
    writer.write("H:/Programmering/AVX-Experimentation/baboon_mine.ascii.pgm", outputImg);

    return 0;
}