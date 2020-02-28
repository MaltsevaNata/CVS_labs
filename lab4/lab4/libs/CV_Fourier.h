#include "global.h"
#include "read&check.h"
#include <complex>

#ifndef _2PI
#define _2PI 6.283185307179586476925286766559
#endif

Mat make_magnitude(Mat complexI);
Mat krasivSpektr(Mat complexI);
Mat fourier_transform(Mat image, Size dftSize);
Mat krasivSpektr1(Mat magI);
Mat my_fourier(Mat image, Size dftSize);
Mat Radix2(Mat image, Size dftSize);
void split(float* input_mas, float* result_mas, int size, int step);
void split_uint(uint8_t* input_mas, float* result_mas, int size, int step);
uint32_t clp2(uint32_t x);
void butterfly(float* XReal, float* XIm, int size, int K);
//static void fastForwardDFT_ForString(uint8_t* inputString, float* resultString, int size, int arrayStep);

//static bool isEven(int number);


//complex_t [0] - REAL
//complex_t [1] -IM



