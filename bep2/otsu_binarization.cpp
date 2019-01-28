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

	int pdf_start = 0, pdf_end = 0;
	int threshold = 0;

	// dynamic binarization; Otsu binarization
	Mat img_otsu(height, width, CV_8UC1);

	float pdf_before[255] = { 0, };
	float arr_pdf[255] = { 0, };
	for (int j = 0; j < height; j++) {	// pdf
		for (int i = 0; i < width; i++) {
			pdf_before[img_gray.at<uchar>(j, i)]++;
		}
	}
	for (int i = 0; i < 256; i++) {
		if (pdf_before[0] != 0) break;
		if (pdf_before[i] == 0) {
			pdf_start = i + 1;
			break;
		}
	}
	for (int i = pdf_start; i < 256; i++) {
		if (pdf_before[i] == 0) {
			pdf_end = i;
			break;
		}
	}
	for (int i = pdf_start; i < pdf_end; i++) {
		arr_pdf[i] = pdf_before[i] / N;
	}

	double w = 0, w0 = 0, w1 = 0;
	double u = 0, u0 = 0, u1 = 0, uT = 0;
	double s0 = 0, s1 = 0, s_W = 0;
	double sigma_w_sqrt = 0, sigma_b_sqrt = 0, sigma_total = 0;
	double v_max = -10000;

	for (int tmp_thres = pdf_start; tmp_thres < pdf_end; tmp_thres++) {
		for (int k = pdf_start; k <= tmp_thres; k++) {
			if (pdf_end - pdf_start != 0) w0 += arr_pdf[k] / (pdf_end - pdf_start);
			if (tmp_thres - pdf_start != 0) u0 += k * arr_pdf[k] / (tmp_thres - pdf_start);
		}
		for (int k = pdf_start; k <= tmp_thres; k++) {
			if (tmp_thres - pdf_start != 0) s0 += powf(k - u0, 2) * arr_pdf[k] / (tmp_thres - pdf_start);
		}

		for (int k = tmp_thres + 1; k < pdf_end; k++) {
			if (pdf_end - pdf_start != 0) w1 += arr_pdf[k] / (pdf_end - pdf_start);
			if (pdf_end - tmp_thres != 0) u1 += k * arr_pdf[k] / (pdf_end - tmp_thres);
		}
		for (int k = tmp_thres + 1; k < pdf_end; k++) {
			if (pdf_end - tmp_thres != 0) s1 += powf(k - u1, 2) * arr_pdf[k] / (pdf_end - tmp_thres);
		}

		sigma_w_sqrt = w0 * s0 + w1 * s1;
		sigma_b_sqrt = w0 * w1 * powf(u1 - u0, 2);
		sigma_total = sigma_w_sqrt - sigma_b_sqrt;
		if (v_max < sigma_b_sqrt) {
			if ((tmp_thres - pdf_start) != 0) v_min = sigma_b_sqrt;
			threshold = tmp_thres;
		}
	}

	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			if (img_gray.at<uchar>(y, x) > threshold) img_otsu.at<uchar>(y, x) = 255;
			else img_otsu.at<uchar>(y, x) = 0;
		}
	}

	// graph of histogram
	Mat img_histo(200, 256, CV_8UC1, Scalar(0));

	int max = -1;
	for (int i = pdf_start; i < 256; i++)
		if (max < pdf_before[i]) max = pdf_before[i];

	for (int i = 0; i < 256; i++)
	{
		int histo = 200 * pdf_before[i] / (float)max;

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

