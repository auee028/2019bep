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

	// Histogram
	int arr_histo[256] = { 0, };
	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			arr_histo[img_gray.at<uchar>(j, i)]++;
		}
	}

	// graph of histogram
	Mat img_histo(300, 600, CV_8UC1, Scalar(0));

	int max = -1;
	for (int i = 0; i < 256; i++)
		if (max < arr_histo[i]) max = arr_histo[i];

	for (int i = 0; i < 256; i++)
	{
		int histo = 300 * arr_histo[i] / (float)max;

		line(img_histo, Point(i + 10, 300), Point(i + 10, 300 - histo), Scalar(255, 255, 255));
	}
	imshow("image", img_gray);
	imshow("histogram", img_histo);
	imwrite("org_histogram.jpg", img_histo);

	waitKey(0);
	return 0;
}

