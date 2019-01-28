#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

using namespace std;
using namespace cv;

float weight(float k) {
	float a = -0.15;
	if (k >= 0 && k < 1) return ((a + 2) * pow(k, 3) - (a + 3) * pow(k, 2) + 1);
	else if (k >= 1 && k < 2) return a * (pow(k, 3) - 5 * pow(k, 2) + 8 * k - 4);
	else return 0;
}

void bicubic(Mat img_in, Mat& img_forward, int multiple) {
	img_forward = Mat(img_in.rows * multiple, img_in.cols * multiple, CV_32F, Scalar(0));
	float d = 1 / (float)multiple;

	for (int j = 0; j < img_in.rows; j++) {
		for (int i = 0; i < img_in.cols; i++) {
			img_forward.at<float>(j * multiple, i * multiple) = img_in.at<uchar>(j, i);
		}
	}

	// x-axe
	for (int j = 0; j < img_in.rows; j++) {
		for (int i = 1; i < img_in.cols - 2; i++) {
			for (int kx = 1; kx < multiple; kx++) {
				img_forward.at<float>(j * multiple, i * multiple + kx)
					= img_in.at<uchar>(j, i - 1) * weight(abs(1 + kx * d))
					+ img_in.at<uchar>(j, i) * weight(abs(kx * d))
					+ img_in.at<uchar>(j, i + 1) * weight(abs(1 - kx * d))
					+ img_in.at<uchar>(j, i + 2) * weight(abs(2 - kx * d));
			}
		}
		for (int kx = 1; kx < multiple; kx++) {
			img_forward.at<float>(j * multiple, kx)
				= 0 * weight(abs(1 + kx * d))
				+ img_in.at<uchar>(j, 0) * weight(abs(kx * d))
				+ img_in.at<uchar>(j, 1) * weight(abs(1 - kx * d))
				+ img_in.at<uchar>(j, 2) * weight(abs(2 - kx * d));

			img_forward.at<float>(j * multiple, (img_in.cols - 2) * multiple + kx)
				= img_in.at<uchar>(j, img_in.cols - 3) * weight(abs(1 + kx * d))
				+ img_in.at<uchar>(j, img_in.cols - 2) * weight(abs(kx * d))
				+ img_in.at<uchar>(j, img_in.cols - 1) * weight(abs(1 - kx * d))
				+ 0 * weight(abs(2 - kx * d));

			img_forward.at<float>(j * multiple, (img_in.cols - 1) * multiple + kx)
				= img_in.at<uchar>(j, img_in.cols - 2) * weight(abs(1 + kx * d))
				+ img_in.at<uchar>(j, img_in.cols - 1) * weight(abs(kx * d))
				+ 0 * weight(abs(1 - kx * d))
				+ 0 * weight(abs(2 - kx * d));
		}
	}/*
	 img_forward.convertTo(img_forward, CV_8U);
	 imshow("x axe", img_forward);
	 img_forward.convertTo(img_forward, CV_32F);*/

	 // y-axe
	for (int i = 0; i < img_forward.cols; i++) {
		for (int j = 1; j < img_in.rows - 2; j++) {
			for (int ky = 1; ky < multiple; ky++) {
				img_forward.at<float>(j * multiple + ky, i)
					= img_forward.at<float>((j - 1) * multiple, i) * weight(abs(1 + ky * d))
					+ img_forward.at<float>(j * multiple, i) * weight(abs(ky * d))
					+ img_forward.at<float>((j + 1) * multiple, i) * weight(abs(1 - ky * d))
					+ img_forward.at<float>((j + 2) * multiple, i) * weight(abs(2 - ky * d));
			}
		}

		for (int ky = 1; ky < multiple; ky++) {
			img_forward.at<float>(ky, i)
				= 0 * weight(abs(1 + ky * d))
				+ img_forward.at<float>(0 * multiple, i) * weight(abs(ky * d))
				+ img_forward.at<float>(1 * multiple, i) * weight(abs(1 - ky * d))
				+ img_forward.at<float>(2 * multiple, i) * weight(abs(2 - ky * d));

			img_forward.at<float>((img_in.rows - 2) * multiple + ky, i)
				= img_forward.at<float>((img_in.rows - 3) * multiple, i) * weight(abs(1 + ky * d))
				+ img_forward.at<float>((img_in.rows - 2) * multiple, i) * weight(abs(ky * d))
				+ img_forward.at<float>((img_in.rows - 1) * multiple, i) * weight(abs(1 - ky * d))
				+ 0 * weight(abs(2 - ky * d));

			img_forward.at<float>((img_in.rows - 1) * multiple + ky, i)
				= img_forward.at<float>((img_in.rows - 2) * multiple, i) * weight(abs(1 + ky * d))
				+ img_forward.at<float>((img_in.rows - 1) * multiple, i) * weight(abs(ky * d))
				+ 0 * weight(abs(1 - ky * d))
				+ 0 * weight(abs(2 - ky * d));
		}
	}

	img_forward.convertTo(img_forward, CV_8U);
}

int main(void) {
	Mat img_in = imread("small.bmp", 0);
	if (img_in.empty()) {
		cout << "Image load failed!" << endl;
		return -1;
	}
	Mat img_out(img_in.size(), CV_32F);

	int multiple = 10;

	bicubic(img_in, img_out, multiple);

	cout << "width: " << img_out.cols << endl
		<< "height: " << img_out.rows << endl;

	imshow("original", img_in);
	imshow("bicubic interpolation", img_out);
	imwrite("bicubic_interpolation.jpg", img_out);

	waitKey(0);
	return 0;
}