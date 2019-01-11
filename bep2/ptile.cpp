/*
too many 'for-state' -> optimiazation would be needed(wrapping up class)
When trying execution, error message of '.exe' is occured.
*/

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
	int sum = 0;
	int p = 2;
	int threshold = 0;

	// dynamic binarization; p-tile binarization
	Mat img_ptile(height, width, CV_8UC1);
	int arr_pdf[255] = { 0, };
	int arr_cdf[255] = { 0, };

	for (int j = 0; j < height; j++) {	// pdf
		for (int i = 0; i < width; i++) {
			arr_pdf[img_gray.at<uchar>(j, i)]++;
		}
	}

	for (int i = 0; i < 256; i++) {		// cdf
		sum += arr_pdf[i];
		arr_cdf[i] = sum;
	}

	int dif_min = height * width + 1;					// large value
	int n = height * width / p;
	for (int i = 0; i < 256; i++) {
		if (dif_min > abs(arr_cdf[i] - n)) {
			dif_min = abs(arr_cdf[i] - n);
			threshold = i;
		}
	}

	for (int j = 0; j < 256; j++) {
		for (int i = 0; i < 256; i++) {
			if (img_gray.at<uchar>(j, i) > threshold) img_ptile.at<uchar>(j, i) = 255;
			else img_ptile.at<uchar>(j, i) = 0;
		}
	}

	imshow("original", img_gray);
	imshow("ptile_2", img_ptile);
	//imwrite("ptile_original.jpg", img_gray);
	imwrite("ptile_2.jpg", img_ptile);		// Don't miss .jpg

	waitKey(0);
	return 0;
}

