#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

using namespace std;
using namespace cv;

void biLinear(Mat img_in, Mat& img_forward, int multiple) {
	img_forward = Mat(img_in.rows * multiple, img_in.cols * multiple, CV_32F, Scalar(0));
	float weight = 1 / (float)multiple;

	for (int j = 0; j < img_in.rows; j++) {
		for (int i = 0; i < img_in.cols; i++) {
			img_forward.at<float>(j * multiple, i * multiple) = img_in.at<uchar>(j, i);
		}
	}

	for (int j = 0; j < img_in.rows; j++) {
		for (int i = 0; i < img_in.cols - 1; i++) {
			for (int kx = 1; kx < multiple; kx++) {
				img_forward.at<float>(j * multiple, i * multiple + kx)
					= (1 - kx * weight) * img_in.at<uchar>(j, i) + kx * weight * img_in.at<uchar>(j, i + 1);
			}
		}
		for (int kx = 1; kx < multiple; kx++)
			img_forward.at<float>(j * multiple, (img_in.cols - 1) * multiple + kx) = (1 - kx * weight) * img_in.at<uchar>(j, img_in.cols - 1) + kx * weight * 0;
	}

	for (int i = 0; i < img_forward.cols; i++) {
		for (int j = 0; j < img_in.rows - 1; j++) {
			for (int ky = 1; ky < multiple; ky++) {
				img_forward.at<float>(j * multiple + ky, i)
					= (1 - ky * weight) * img_forward.at<float>(j * multiple, i) + ky * weight * img_forward.at<float>((j + 1) * multiple, i);
			}
		}
		for (int ky = 1; ky < multiple; ky++)
			img_forward.at<float>((img_in.rows - 1) * multiple + ky, i) = (1 - ky * weight) * 0 + ky * weight * img_forward.at<float>(img_in.rows - 1, i);
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

	biLinear(img_in, img_out, multiple);

	cout << "width: " << img_out.cols << endl
		<< "height: " << img_out.rows << endl;

	imshow("original", img_in);
	imshow("bilinear interpolation", img_out);
	imwrite("bilinear_interpolation.jpg", img_out);

	waitKey(0);
	return 0;
}