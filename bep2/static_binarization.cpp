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

	int threshold = 220;

	// static binarization
	Mat img_stat_bin(height, width, CV_8UC1);

	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			if (img_gray.at<uchar>(j, i) > threshold) img_stat_bin.at<uchar>(j, i) = 255;
			else img_stat_bin.at<uchar>(j, i) = 0;
		}
	}

	imshow("original", img_gray);
	imshow("stat_binarize_220", img_stat_bin);
	//imwrite("binarization_original.jpg", img_gray);
	imwrite("static_binarzation_220.jpg", img_stat_bin);		// Don't miss .jpg

	waitKey(0);
	return 0;
}

