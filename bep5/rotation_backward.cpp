#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

using namespace std;
using namespace cv;

void rotation(Mat img_in, Mat& img_out, float theta) {
	int w0 = img_in.cols, h0 = img_in.rows;

	float x[4] = { 0, };
	x[0] = cos(theta) * (0) - sin(theta) * (0);
	x[1] = cos(theta) * (0) - sin(theta) * (h0 - 1);
	x[2] = cos(theta) * (w0 - 1) - sin(theta) * (0);
	x[3] = cos(theta) * (w0 - 1) - sin(theta) * (h0 - 1);

	float y[4] = { 0, };
	y[0] = sin(theta) * (0) + cos(theta) * (0);
	y[1] = sin(theta) * (0) + cos(theta) * (h0 - 1);
	y[2] = sin(theta) * (w0 - 1) + cos(theta) * (0);
	y[3] = sin(theta) * (w0 - 1) + cos(theta) * (h0 - 1);

	float x_max = x[0], x_min = x[0], y_max = y[0], y_min = y[0];
	for (int i = 1; i < 4; i++) {
		if (x_max < x[i]) x_max = x[i];
		if (x_min > x[i]) x_min = x[i];
		if (y_max < y[i]) y_max = y[i];
		if (y_min > y[i]) y_min = y[i];
	}

	int w = ceil(abs(x_max - x_min));
	int h = ceil(abs(y_max - y_min));
	int x_original = 0, y_original = 0;

	img_out = Mat(h, w, CV_32F, Scalar(0));

	for (int j = 0; j < h; j++) {			// backward
		for (int i = (int)x_min; i < w + (int)x_min; i++) {
			x_original = floor(cos(theta) * i + sin(theta) * j);
			y_original = floor((-sin(theta)) * i + cos(theta) * j);
			if (y_original < 0 || x_original < 0 || y_original > h0 - 1 || x_original > w0 - 1)
				continue;
			else
				img_out.at<float>(j, i - (int)x_min) = img_in.at<uchar>(y_original, x_original);
		}
	}
	img_out.convertTo(img_out, CV_8U);
}

int main(void) {
	Mat img_in = imread("Woman.bmp", 0);
	if (img_in.empty()) {
		cout << "Image load failed!" << endl;
		return -1;
	}
	Mat img_out;

	float degree = 30;
	float theta = degree * CV_PI / 180;

	rotation(img_in, img_out, theta);

	imshow("original", img_in);
	imshow("rotation(backward matrix)", img_out);
	imwrite("rotation_backward.jpg", img_out);

	waitKey(0);
	return 0;
}