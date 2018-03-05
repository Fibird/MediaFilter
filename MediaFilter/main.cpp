#include "stdafx.h"
#include "medianfilter.h"
#include <opencv2\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main(int argc, char** argv)
{
	Mat ImgSrc = imread("scratch_noises.bmp", CV_LOAD_IMAGE_GRAYSCALE);

	if (!ImgSrc.data)          // Check for invalid input
	{
		cout << "Could not open or find the image" << std::endl;
		return -1;
	}

	Mat ImgDst = ImgSrc.clone();
	int imgSize = ImgSrc.cols * ImgSrc.rows;	// Get the Image size

	unsigned char *pSrcData = (unsigned char*)(ImgSrc.data);
	unsigned char *pDstData = (unsigned char*)(ImgDst.data);

	// Call median filter 5 times
	for (int i = 0; i < 5; ++i)
	{
		medianfilter(pSrcData, pDstData, ImgSrc.cols, ImgSrc.rows);
	}
	imwrite("processed_image.bmp", ImgDst);
	namedWindow("Original Image");
	imshow("Original Image", ImgSrc);
	namedWindow("Processed Image");
	imshow("Processed Image", ImgDst);
	cvWaitKey();
	return 0;
}