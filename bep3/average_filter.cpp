#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

void correlation(Mat img_in, Mat& img_out, Mat mask) {
	img_out = Mat(img_in.size(), CV_32F, Scalar(0));
	Point center_mask = mask.size() / 2;

	for (int j = center_mask.y; j < img_in.rows - center_mask.y; j++) {
		for (int i = center_mask.x; i < img_in.cols - center_mask.x; i++) {
			float sum = 0;
			for (int v = 0; v < mask.rows; v++) {
				for (int u = 0; u < mask.cols; u++) {
					int y = j + v - center_mask.y;
					int x = i + u - center_mask.x;
					sum += mask.at<float>(v, u) * img_in.at<uchar>(y, x);
				}
			}
			img_out.at<float>(j, i) = sum;
		}
	}
}

void convolution(Mat img_in, Mat& img_out, Mat mask) {
	img_out = Mat(img_in.size(), CV_32F, Scalar(0));
	Point center_mask = mask.size() / 2;

	for (int j = center_mask.y; j < img_in.rows - center_mask.y; j++) {
		for (int i = center_mask.x; i < img_in.cols - center_mask.x; i++) {
			float sum = 0;
			for (int v = 0; v < mask.rows; v++) {
				for (int u = 0; u < mask.cols; u++) {
					int y = j + v - center_mask.y;
					int x = i + u - center_mask.x;
					sum += mask.at<float>(mask.rows - 1 - v, mask.cols - 1 - u) * img_in.at<uchar>(y, x);
				}
			}
			img_out.at<float>(j, i) = sum;
		}
	}
}

void zeroPad(Mat img_in, Mat& img_padding, int mask_size) {

	int height = img_in.rows;
	int width = img_in.cols;

	img_padding = Mat(height + (mask_size - 1), width + (mask_size - 1), CV_8UC1, Scalar(0));

	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			img_padding.at<uchar>(j + (mask_size - 1) / 2, i + (mask_size - 1) / 2) = img_in.at<uchar>(j, i);
		}
	}

	imshow("zero padding", img_padding);

	waitKey(0);
}

int main(void) {
	Mat img_gray = imread("Test1.bmp", 0);
	if (img_gray.empty()) {
		cerr << "Image load failed!" << endl;
		return -1;
	}
	int mask_size = 3;
	Mat img_padding;

	zeroPad(img_gray, img_padding, mask_size);

	float data_average[] = {
		1 / 9.f, 1 / 9.f, 1 / 9.f,		// 1/9 -> result is 0 because 9 is int.(result is the quotient of 1/9)
		1 / 9.f, 1 / 9.f, 1 / 9.f,
		1 / 9.f, 1 / 9.f, 1 / 9.f
	};

	Mat average_filter(3, 3, CV_32F, data_average);
	Mat img_out;
	convolution(img_gray, img_out, average_filter);
	img_out.convertTo(img_out, CV_8U);

	imshow("original", img_gray);
	imshow("average filter", img_out);
	imwrite("average_filter.jpg", img_out);

	waitKey(0);
	return 0;
}