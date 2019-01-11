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
	int threshold = 1;

	// dynamic binarization; Otsu binarization
	Mat img_otsu(height, width, CV_8UC1);

	float pdf_before[255] = { 0, };
	float arr_pdf[255] = { 0, };
	for (int j = 0; j < height; j++) {	// pdf
		for (int i = 0; i < width; i++) {
			pdf_before[img_gray.at<uchar>(j, i)]++;
			arr_pdf[img_gray.at<uchar>(j, i)]++;
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

	//
	double tot[255] = { 0, };
	for (int t = 1; t < pdf_end; t++)
	{
		double rsum = 0;
		double lsum = 0;

		for (int i = 0; i < 255; i++)
		{
			if (i < t) {
				lsum += i * arr_pdf[i];
			}
			else
			{
				rsum += i * arr_pdf[i];
			}
		}

		double a = lsum / (lsum + rsum);
		double b = 1 - a;
		double lmean = lsum / t;
		double rmean = rsum / (255 - t);
		double lvar = 0;
		double rvar = 0;

		for (int i = 0; i < 255; i++)
		{
			if (i < t) {
				lvar += pow(arr_pdf[i] - lmean, 2);
			}
			else
			{
				rvar += pow(arr_pdf[i] - rmean, 2);
			}
		}

		lvar = lvar / t;
		rvar = rvar / (255 - t);
		double pmean = pow(lmean - rmean, 2);
		double invar = a * lvar + b * rvar;
		double betvar = a * b * pmean;
		double totvar = invar + betvar;
		tot[t] = betvar;
	}

	double vmin = 0;
	int mint = 0;

	for (int i = 1; i < 255; i++)
	{
		if (tot[i] > vmin)
		{
			vmin = tot[i];
			mint = i;
		}
		else
			vmin = vmin;
	}


	for (int x = 0; x < img_gray.cols; x++)
	{
		for (int y = 0; y < img_gray.rows; y++)
		{
			int pixel = img_gray.at<uchar>(y, x);

			if (pixel > mint)
				pixel = 255;
			else
				pixel = 0;

			img_otsu.at<uchar>(y, x) = pixel;
		}
	}
	cout << "t: " << mint << endl;

	//double leftsum = 0, rightsum = 0;
	//double w0 = 0, w1 = 0;
	//double u0 = 0, u1 = 0, uT = 0;
	//double s0 = 0, s1 = 0;
	//double var_within = 0, var_between = 0, var_total = 0;
	//double v_min = 9999999;

	//for (int tmp_thres = pdf_start; tmp_thres < pdf_end; tmp_thres++) {
	//	for (int k = pdf_start; k <= tmp_thres; k++) {
	//		w0 += arr_pdf[k];
	//		u0 += k * arr_pdf[k];
	//	}
	//	leftsum = w0;
	//	rightsum = N - leftsum;
	//	w0 = w0 / N;
	//	u0 = u0 / leftsum;
	//	for (int k = pdf_start; k <= tmp_thres; k++) {
	//		s0 += powf(k - u0, 2) * arr_pdf[k];
	//	}
	//	s0 = s0 / leftsum;

	//	w1 = 1 - w0;
	//	for (int k = tmp_thres + 1; k < pdf_end; k++) {
	//		u1 += k * arr_pdf[k];
	//	}
	//	u1 = u1 / rightsum;
	//	for (int k = tmp_thres + 1; k < pdf_end; k++) {
	//		s1 += powf(k - u1, 2) * arr_pdf[k];
	//	}
	//	s1 = s1 / rightsum;

	//	var_within = w0 * s0 + w1 * s1;
	//	var_between = w0 * w1 * powf(u1 - u0, 2);
	//	var_total = var_within + var_between;
	//	if (v_min > var_total) {
	//		v_min = var_total;
	//		threshold = tmp_thres;
	//	}
	//}

	//for (int y = 0; y < height; y++) {
	//	for (int x = 0; x < width; x++) {
	//		if (img_gray.at<uchar>(y, x) > threshold) img_otsu.at<uchar>(y, x) = 255;
	//		else img_otsu.at<uchar>(y, x) = 0;
	//	}
	//}

	// graph of histogram
	Mat img_histo(200, 256, CV_8UC1, Scalar(0));

	int max = -1;
	for (int i = pdf_start; i < 256; i++)
		if (max < pdf_before[i]) max = pdf_before[i];

	for (int i = 0; i < 256; i++)
	{
		int histo = 200 * pdf_before[i] / (float)max;

		line(img_histo, Point(i, 200), Point(i, 200 - histo), Scalar(255, 255, 255));
		line(img_histo, Point(mint, 0), Point(mint, 255), Scalar(200, 200, 200));
	}

	//cout << "threshold = " << threshold << endl;

	imshow("original", img_gray);
	imshow("otsu binarization", img_otsu);
	imshow("histogram", img_histo);
	//imwrite("otsu_original.jpg", img_gray);
	//imwrite("otsu_binarization.jpg", img_otsu);		// Don't miss .jpg
	//imwrite("otsu_histogram.jpg", img_histo);

	waitKey(0);
	return 0;
}

