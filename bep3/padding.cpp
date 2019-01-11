#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

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


void copyPad(Mat img_in, Mat& img_padding, int mask_size) {

	int height = img_in.rows;
	int width = img_in.cols;

	int half = (mask_size - 1) / 2;

	img_padding = Mat(height + (mask_size - 1), width + (mask_size - 1), CV_8UC1, Scalar(0));

	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			img_padding.at<uchar>(j + half, i + half) = img_in.at<uchar>(j, i);
		}
	}

	for (int j = 0; j < height; j++) {
		for (int i = 0; i < (mask_size - 1) / 2; i++) {
			img_padding.at<uchar>(j + half, i) = img_in.at<uchar>(j, i);
			img_padding.at<uchar>(j + half, i + width + half) = img_in.at<uchar>(j, i + width - half);
		}
	}

	for (int j = 0; j < (mask_size - 1) / 2; j++) {
		for (int i = 0; i < width; i++) {
			img_padding.at<uchar>(j, i + half) = img_in.at<uchar>(j, i);
			img_padding.at<uchar>(j + height + half, i + half) = img_in.at<uchar>(j + height - half, i);
		}
		for (int i = 0; i < half; i++)   // 모서리 그대로 복사
		{
			img_padding.at<uchar>(i, j) = img_in.at<uchar>(i, j);
			img_padding.at<uchar>(height + half + i, width + half + j) = img_in.at<uchar>(height - half + i, width - half + j);
			img_padding.at<uchar>(height + half + i, j) = img_in.at<uchar>(height - half + i, j);
			img_padding.at<uchar>(i, width + half + j) = img_in.at<uchar>(i, width - half + j);
		}
	}

	imshow("copy padding", img_padding);

	waitKey(0);
}

void reflectPad(Mat img_in, Mat& img_padding, int mask_size) {

	int height = img_in.rows;
	int width = img_in.cols;

	int half = (mask_size - 1) / 2;

	img_padding = Mat(height + (mask_size - 1), width + (mask_size - 1), CV_8UC1, Scalar(0));

	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			img_padding.at<uchar>(j + half, i + half) = img_in.at<uchar>(j, i);
		}
	}

	for (int j = 0; j < height; j++) {
		for (int i = 0; i < half; i++) {
			img_padding.at<uchar>(j + half, half - 1 - i) = img_in.at<uchar>(j, i);
			img_padding.at<uchar>(j + half, width + half + (half - 1 - i)) = img_in.at<uchar>(j, i + width - half);
		}
	}

	for (int j = 0; j < (mask_size - 1) / 2; j++) {
		for (int i = 0; i < width; i++) {
			img_padding.at<uchar>(half - 1 - j, i + half) = img_in.at<uchar>(j, i);
			img_padding.at<uchar>(height + half + (half - 1 - j), i + half) = img_in.at<uchar>(j + height - half, i);
		}
		for (int i = 0; i < half; i++)   // 모서리 그대로 복사
		{
			img_padding.at<uchar>(i, j) = img_in.at<uchar>(i, j);
			img_padding.at<uchar>(height + half + i, width + half + j) = img_in.at<uchar>(height - half + i, width - half + j);
			img_padding.at<uchar>(height + half + i, j) = img_in.at<uchar>(height - half + i, j);
			img_padding.at<uchar>(i, width + half + j) = img_in.at<uchar>(i, width - half + j);
		}
	}

	imshow("reflection padding", img_padding);

	waitKey(0);
}

void wrapAroundPad(Mat img_in, Mat& img_padding, int mask_size) {

	int height = img_in.rows;
	int width = img_in.cols;

	img_padding = Mat(height + (mask_size - 1), width + (mask_size - 1), CV_8UC1);

	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			img_padding.at<uchar>(j + (mask_size - 1) / 2, i + (mask_size - 1) / 2) = img_in.at<uchar>(j, i);
		}
	}

	for (int j = 0; j < (mask_size - 1) / 2; j++) {
		for (int i = 0; i < width; i++) {
			img_padding.at<uchar>(j, i) = img_in.at<uchar>(j, i);
			img_padding.at<uchar>(j + height, i) = img_in.at<uchar>(j + height - (mask_size - 1) / 2, i);
		}
	}

	for (int j = 0; j < height + (mask_size - 1); j++) {
		for (int i = (mask_size - 1) / 2; i < (mask_size - 1); i++) {
			img_padding.at<uchar>(j, i - (mask_size - 1) / 2) = img_padding.at<uchar>(j, i + width - (mask_size - 1) / 2);
			img_padding.at<uchar>(j, i + width) = img_padding.at<uchar>(j, i);
		}
	}

	imshow("wrap around padding", img_padding);

	waitKey(0);
}

int main(void) {	// padding

	int mask_size = 3;

	Mat img_gray = imread("Test2.bmp", 0);
	if (img_gray.empty()) {
		cerr << "Image load failed!" << endl;
		return -1;
	}
	Mat img_out;

	zeroPad(img_gray, img_out, mask_size);
	copyPad(filename, mask_size);
	//reflectPad(filename, mask_size);
	//wrapAroundPad(filename, mask_size);

	return 0;
}