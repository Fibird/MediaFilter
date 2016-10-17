// MediaFilter.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

//   medianfilter.cpp - impelementation of 
//   1D and 2D median filter routines
//
//   The code is property of LIBROW
//   You can use it on your own
//   When utilizing credit LIBROW site

#include <memory.h>
#include "medianfilter.h"
#include <opencv2\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <iostream>

using namespace std;
using namespace cv;

//   1D MEDIAN FILTER implementation
//     signal - input signal
//     result - output signal
//     N      - length of the signal
void _medianfilter(const element* signal, element* result, int N)
{
	//   Move window through all elements of the signal
	for (int i = 2; i < N - 2; ++i)
	{
		//   Pick up window elements
		element window[5];
		for (int j = 0; j < 5; ++j)
			window[j] = signal[i - 2 + j];
		//   Order elements (only half of them)
		for (int j = 0; j < 3; ++j)
		{
			//   Find position of minimum element
			int min = j;
			for (int k = j + 1; k < 5; ++k)
				if (window[k] < window[min])
					min = k;
			//   Put found minimum element in its place
			const element temp = window[j];
			window[j] = window[min];
			window[min] = temp;
		}
		//   Get result - the middle element
		result[i - 2] = window[2];
	}
}

//   1D MEDIAN FILTER wrapper
//     signal - input signal
//     result - output signal
//     N      - length of the signal
void medianfilter(element* signal, element* result, int N)
{
	//   Check arguments
	if (!signal || N < 1)
		return;
	//   Treat special case N = 1
	if (N == 1)
	{
		if (result)
			result[0] = signal[0];
		return;
	}
	//   Allocate memory for signal extension
	element* extension = new element[N + 4];
	//   Check memory allocation
	if (!extension)
		return;
	//   Create signal extension
	memcpy(extension + 2, signal, N * sizeof(element));
	for (int i = 0; i < 2; ++i)
	{
		extension[i] = signal[1 - i];
		extension[N + 2 + i] = signal[N - 1 - i];
	}
	//   Call median filter implementation
	_medianfilter(extension, result ? result : signal, N + 4);
	//   Free memory
	delete[] extension;
}

//   2D MEDIAN FILTER implementation
//     image  - input image
//     result - output image
//     N      - width of the image
//     M      - height of the image
void _medianfilter(const element* image, element* result, int N, int M)
{
	//   Move window through all elements of the image
	for (int m = 1; m < M - 1; ++m)
		for (int n = 1; n < N - 1; ++n)
		{
			//   Pick up window elements
			int k = 0;
			element window[9];
			for (int j = m - 1; j < m + 2; ++j)
				for (int i = n - 1; i < n + 2; ++i)
					window[k++] = image[j * N + i];
			//   Order elements (only half of them)
			for (int j = 0; j < 5; ++j)
			{
				//   Find position of minimum element
				int min = j;
				for (int l = j + 1; l < 9; ++l)
					if (window[l] < window[min])
						min = l;
				//   Put found minimum element in its place
				const element temp = window[j];
				window[j] = window[min];
				window[min] = temp;
			}
			//   Get result - the middle element
			result[(m - 1) * (N - 2) + n - 1] = window[4];
		}
}

//   2D MEDIAN FILTER wrapper
//     image  - input image
//     result - output image
//     N      - width of the image
//     M      - height of the image
void medianfilter(element* image, element* result, int N, int M)
{
	//   Check arguments
	if (!image || N < 1 || M < 1)
		return;
	//   Allocate memory for signal extension
	element* extension = new element[(N + 2) * (M + 2)];
	//   Check memory allocation
	if (!extension)
		return;
	//   Create image extension
	for (int i = 0; i < M; ++i)
	{
		memcpy(extension + (N + 2) * (i + 1) + 1, image + N * i, N * sizeof(element));
		extension[(N + 2) * (i + 1)] = image[N * i];
		extension[(N + 2) * (i + 2) - 1] = image[N * (i + 1) - 1];
	}
	//   Fill first line of image extension
	memcpy(extension, extension + N + 2, (N + 2) * sizeof(element));
	//   Fill last line of image extension
	memcpy(extension + (N + 2) * (M + 1), extension + (N + 2) * M, (N + 2) * sizeof(element));
	//   Call median filter implementation
	_medianfilter(extension, result ? result : image, N + 2, M + 2);
	//   Free memory
	delete[] extension;
}


int main()
{
	IplImage *ImgSrc = cvLoadImage("sample_corrupted.bmp", CV_LOAD_IMAGE_GRAYSCALE);
	IplImage *ImgReal = cvLoadImage("sample.bmp", CV_LOAD_IMAGE_GRAYSCALE);
	IplImage *ImgDst_CPU = cvCreateImage(cvGetSize(ImgSrc), IPL_DEPTH_8U, 1);
	int Size = ImgSrc->width * ImgSrc->height;
	
	unsigned char *pSrcData = (unsigned char*)(ImgSrc->imageData);
	unsigned char *pDstData = (unsigned char*)(ImgDst_CPU->imageData);
	for (int i = 0; i < 20; ++i)
	{
		medianfilter(pSrcData, pDstData, Size);
		//medianfilter(pSrcData, pDstData, ImgSrc->width, ImgSrc->height);
	}
		
	cvNamedWindow("Real");
	cvShowImage("Real", ImgReal);
	cvNamedWindow("Src");
	cvShowImage("Src", ImgSrc);
	cvNamedWindow("Dst");
	cvShowImage("Dst", ImgDst_CPU);
	cvWaitKey();
    return 0;
}

