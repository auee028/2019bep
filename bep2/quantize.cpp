#include "opencv2/opencv.hpp"
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>

using namespace cv;
using namespace std;

int main(void)
{
	Mat img_gray = imread("LENA256.jpg", 0);	// LENA256.jpg is not a grayscale image(true color image) -> without second arg., only one third of the image is processed.
	if (img_gray.empty()) {
		cerr << "Image load failed!" << endl;
		return -1;
	}

	int width = img_gray.cols;
	int height = img_gray.rows;
	int temp = 0;

	int level = 16;

	// static binarization
	Mat img_quantize(height, width, CV_8UC1);

	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			temp = trunc(img_gray.at<uchar>(j, i) / (256 / level));		// pixel is divided by the size of interval
			img_quantize.at<uchar>(j, i) = (int)(temp * 255 / (level - 1));		// quantization index is muliplied with the value of coresponding level
		}
	}

	imshow("original", img_gray);
	imshow("quantize_16", img_quantize);
	//imwrite("quantize_original.jpg", img_gray);
	imwrite("quantize_16.jpg", img_quantize);		// Don't miss .jpg

	waitKey(0);
	return 0;
}

