/* Author Jens Edhammer
*/
#include <iostream>
#include <string>
#include <immintrin.h>
#include "pgm.hpp"

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
    SimpleImage* img = pgmReader.read("H:/Programmering/SSEGuide/testData/baboon.ascii.pgm");
    std::cout << "Width: "  <<  img->getImageInfo().width  <<  " Height: "  << img->getImageInfo().height;

    //Process the image here. 

    PGMWriter writer;
    writer.write("H:/Programmering/SSEGuide/baboon_mine.ascii.pgm", *img);

    return 0;
}