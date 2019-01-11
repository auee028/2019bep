#include "opencv2/opencv.hpp"
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>

using namespace cv;
using namespace std;

int main(void)
{
	Mat img_gray = imread("LENA256.jpg", IMREAD_GRAYSCALE);
	if (img_gray.empty()) {
		cerr << "Image load failed!" << endl;
		return -1;
	}

	int height = img_gray.rows;
	int width = img_gray.cols;
	int temp, slided = 0;

	// Sliding
	Mat img_sliding(height, width, CV_8UC1);
	int arr_histo[256] = { 0, };
	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			temp = img_gray.at<uchar>(j, i) + 0;				// step1. image processing
			img_sliding.at<uchar>(j, i) = temp;
			if (temp > 255) img_sliding.at<uchar>(j, i) = 255;
			if (temp < 0) img_sliding.at<uchar>(j, i) = 0;

			arr_histo[img_sliding.at<uchar>(j, i)]++;			// step2. array for histogram
		}
	}

	// graph of histogram
	Mat img_histo(200, 256, CV_8UC1, Scalar(0));

	int max = -1;
	for (int i = 0; i < 256; i++)
		if (max < arr_histo[i]) max = arr_histo[i];

	for (int i = 0; i < 256; i++)
	{
		int histo = 200 * arr_histo[i] / (float)max;

		line(img_histo, Point(i, 200), Point(i, 200 - histo), Scalar(255, 255, 255));
	}
	imshow("original", img_gray);
	imshow("image with sliding", img_sliding);
	imshow("histo_original", img_histo);
	//imwrite("sliding_0.jpg", img_sliding);
	//imwrite("histo_original.jpg", img_histo);

	waitKey(0);
	return 0;
}

