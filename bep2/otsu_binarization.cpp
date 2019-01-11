#include "opencv2/opencv.hpp"
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>

using namespace cv;
using namespace std;

int main(void)
{
	Mat img_gray = imread("girl.jpg", 0);
	if (img_gray.empty()) {
		cerr << "Image load failed!" << endl;
		return -1;
	}

	int width = img_gray.cols;
	int height = img_gray.rows;
	int N = height * width;

	int threshold = 0;

	// dynamic binarization; Otsu binarization
	Mat img_otsu(height, width, CV_8UC1);

	float arr_pdf[255] = { 0, };
	for (int j = 0; j < height; j++) {	// pdf
		for (int i = 0; i < width; i++) {
			arr_pdf[img_gray.at<uchar>(j, i)]++;
		}
	}

	float w0 = 0; float w1 = 0;
	float u0 = 0; float u1 = 0; float uT = 0;
	float sigma_b_sqrt = 0; float sigma_T_sqrt = 0;
	float eta = 0; float v_max = -1;
	for (int pix = 0; pix < 256; pix++) {
		for (int i = 0; i < pix; i++) {
			w0 += (arr_pdf[i] / N);
			u0 += i * (arr_pdf[i] / N);
		}
		w1 = 1 - w0;

		float temp = u0;
		if (w0 != 0) u0 = temp / w0;
		else u0 = 0;
		for (int i = 0; i < 256; i++)
			uT += i * (arr_pdf[i] / N);
		u1 = (uT - temp) / w1;

		sigma_b_sqrt = w0 * w1 * powf(u0 - u1, 2);
		for (int i = 0; i < 256; i++)
			sigma_T_sqrt += powf(i - uT, 2) * (arr_pdf[i] / N);

		eta = sigma_b_sqrt / sigma_T_sqrt;
		if (v_max < eta) {
			v_max = eta;
			threshold = pix;
		}
	}

	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			if (img_gray.at<uchar>(j, i) > threshold) img_otsu.at<uchar>(j, i) = 255;
			else img_otsu.at<uchar>(j, i) = 0;
		}
	}

	// graph of histogram
	Mat img_histo(200, 256, CV_8UC1, Scalar(0));

	int max = -1;
	for (int i = 0; i < 256; i++)
		if (max < arr_pdf[i]) max = arr_pdf[i];

	for (int i = 0; i < 256; i++)
	{
		int histo = 200 * arr_pdf[i] / (float)max;

		line(img_histo, Point(i, 200), Point(i, 200 - histo), Scalar(255, 255, 255));
		line(img_histo, Point(threshold, 0), Point(threshold, 255), Scalar(200, 200, 200));
	}

	cout << "threshold = " << threshold << endl;

	imshow("original", img_gray);
	imshow("otsu binarization", img_otsu);
	imshow("histogram", img_histo);
	//imwrite("otsu_original.jpg", img_gray);
	//imwrite("otsu_binarization.jpg", img_otsu);		// Don't miss .jpg
	//imwrite("otsu_histogram.jpg", img_histo);

	waitKey(0);
	return 0;
}

