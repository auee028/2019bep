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
	int h_min = 500;
	int h_max = -1;
	int temp;

	// Stretching
	Mat img_stretching(height, width, CV_8UC1);
	int arr_histo[256] = { 0, };
	int arr_histo_stretching[256] = { 0, };

	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			if (h_min > img_gray.at<uchar>(j, i)) h_min = img_gray.at<uchar>(j, i);
			if (h_max < img_gray.at<uchar>(j, i)) h_max = img_gray.at<uchar>(j, i);
			arr_histo[img_gray.at<uchar>(j, i)]++;
		}
	}

	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			temp = img_gray.at<uchar>(j, i);
			img_stretching.at<uchar>(j, i) = (int)((temp - h_min) * 255.0 / (h_max - h_min));
			arr_histo_stretching[img_stretching.at<uchar>(j, i)]++;
		}
	}

	// graph of histogram
	Mat img_histo1(200, 256, CV_8UC1, Scalar(0));
	Mat img_histo2(200, 256, CV_8UC1, Scalar(0));

	int max1 = -1;
	int max2 = -1;
	for (int i = 0; i < 256; i++) {
		if (max1 < arr_histo[i]) max1 = arr_histo[i];
		if (max2 < arr_histo_stretching[i]) max2 = arr_histo_stretching[i];
	}

	for (int i = 0; i < 256; i++)
	{
		int histo1 = 200 * arr_histo[i] / (float)max1;
		int histo2 = 200 * arr_histo_stretching[i] / (float)max2;

		line(img_histo1, Point(i, 200), Point(i, 200 - histo1), Scalar(255, 255, 255));
		line(img_histo2, Point(i, 200), Point(i, 200 - histo2), Scalar(255, 255, 255));
	}
	imshow("original", img_gray);
	imshow("stretching", img_stretching);
	imshow("histo_original", img_histo1);
	imshow("histo_stretching", img_histo2);
	imwrite("img_stretching.jpg", img_stretching);
	imwrite("histo_stretching.jpg", img_histo2);

	waitKey(0);
	return 0;
}

