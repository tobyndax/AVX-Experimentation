/* Author Jens Edhammer
*/
#include "ex1_rawIntrinsics.h"
#include "ex2_wrappedIntrinsics.h"
#include <iostream>

/*  This small code base contains a PlainPGM (ASCII "P2") reader and writer to easily get images into the program
 *  and to easily output them to file again.
 *  The idea with the code is to facilitate easy experimentation with SIMD optimization techniques
 *
 *	The image load and save functions are to help with experimenting with image processing algos and SIMD.
 */

 //Create some float data and process it with raw intrinsics. The performance measurements are of course only valid in release.
 //int main() {
 //	ex2::runExampleTwo();
 //
 //	std::cin.ignore();
 //	return 0;
 //}

//Give this link a read: https://www.codeproject.com/Articles/874396/Crunching-Numbers-with-AVX-and-AVX



int main() {
	float data[8]{ 0,1,2,3,4,5,6,7 };

	__m256 floatSimdRegister = _mm256_loadu_ps(data); //load unaligned precision single. "float"
	//__m256 floatSimdRegister = _mm256_load_ps(data); // there is also an aligned version, then the input data need to be aligned to 64-bit barriers.
	// We can do this like this to create some aligned data 
	alignas(64) float* dataAligned = new float[8]; //Create some aligned memory.

	//we can add simd-registers together
	__m256 addedRegisters = _mm256_add_ps(floatSimdRegister, floatSimdRegister);
	//We can of course do other arithmetic ops. 
	__m256 multipliedRegisters = _mm256_mul_ps(floatSimdRegister, floatSimdRegister); // note division with zero
	__m256 dividedRegisters = _mm256_div_ps(floatSimdRegister, floatSimdRegister);
	__m256 subtractedRegisters = _mm256_sub_ps(floatSimdRegister, floatSimdRegister);

	// a strong mechanism in simd is the fairly modern 'blend' function to get 'conditional' calculations.
	// add one to values larger than 3, subtract from those smaller than 4
	__m256 threes = _mm256_set1_ps(3);

	//magic number 14 for cmp greater than. (not really, see https://software.intel.com/sites/landingpage/IntrinsicsGuide/#expand=443,451,738&techs=AVX,AVX2&text=_mm256_cmp)
	__m256 mask = _mm256_cmp_ps(floatSimdRegister, threes, 14);
	//Perform both calculation paths alá GPU-programming
	__m256 subs = _mm256_sub_ps(floatSimdRegister,_mm256_set1_ps(1)); //it's okay to do inlined as well ofc. 
	__m256 adds = _mm256_add_ps(floatSimdRegister, _mm256_set1_ps(1)); //it's okay to do inlined as well ofc. 
	
	//now reconstruct our wanted data. 
	__m256 results = _mm256_blendv_ps(subs, adds, mask);
}