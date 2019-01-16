/* Author Jens Edhammer
*/
#include "ex1_rawIntrinsics.h"
#include <iostream>

/*  This small code base contains a PlainPGM (ASCII "P2") reader and writer to easily get images into the program
 *  and to easily output them to file again. 
 *  The idea with the code is to facilitate easy experimentation with SIMD optimization techniques
 *
 *	The image load and save functions are to help with experimenting with image processing algos and SIMD.
 */

//Create some float data and process it with raw intrinsics.
int main() {
	runExampleOne();

	std::cin.ignore();
	return 0;
}
