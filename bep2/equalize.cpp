#include "opencv2/opencv.hpp"
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>

using namespace cv;
using namespace std;

int main(void)
{
	Mat img_gray = imread("BEPE.jpg", IMREAD_GRAYSCALE);
	if (img_gray.empty()) {
		cerr << "Image load failed!" << endl;
		return -1;
	}

	int height = img_gray.rows;
	int width = img_gray.cols;
	int cdf_min = 500;
	int cdf_max = -1;
	int sum = 0;

	// Equalize
	Mat img_equalize(height, width, CV_8UC1);
	int arr_pdf[256] = { 0, };
	int arr_cdf[256] = { 0, };
	int arr_pdf_e[256] = { 0, };
	int arr_cdf_e[256] = { 0, };

	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			arr_pdf[img_gray.at<uchar>(j, i)]++;
		}
	}

	for (int i = 0; i < 256; i++) {
		sum += arr_pdf[i];
		arr_cdf[i] = sum;
	}

	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			img_equalize.at<uchar>(j, i) = (int)(arr_cdf[img_gray.at<uchar>(j, i)] * 255 / arr_cdf[255]);
			arr_pdf_e[img_equalize.at<uchar>(j, i)]++;
		}
	}

	for (int i = 0; i < 256; i++) {
		sum += arr_pdf_e[i];
		arr_cdf_e[i] = sum;
	}

	// graph of histogram
	Mat img_histo1(200, 256, CV_8UC1, Scalar(0));
	Mat img_histo2(200, 256, CV_8UC1, Scalar(0));

	int max1 = -1;
	int max2 = -1;
	for (int i = 0; i < 256; i++) {
		if (max1 < arr_cdf[i]) max1 = arr_cdf[i];
		if (max2 < arr_cdf_e[i]) max2 = arr_cdf_e[i];
	}

	for (int i = 0; i < 256; i++)
	{
		int histo1 = 200 * arr_cdf[i] / (float)max1;
		int histo2 = 200 * arr_cdf_e[i] / (float)max2;

		line(img_histo1, Point(i, 200), Point(i, 200 - histo1), Scalar(255, 255, 255));
		line(img_histo2, Point(i, 200), Point(i, 200 - histo2), Scalar(255, 255, 255));
	}
	imshow("original", img_gray);
	imshow("equalize", img_equalize);
	imshow("cdf_original", img_histo1);
	imshow("cdf_equalize", img_histo2);
	//imwrite("img_equalize.jpg", img_equalize);
	//imwrite("cdf_equalize.jpg", img_histo2);

	waitKey(0);
	return 0;
}

