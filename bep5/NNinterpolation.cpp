#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

using namespace std;
using namespace cv;

float weight(float k) {
	if (k >= 0 && k < 1) return ((1.0 / 2) * pow(k, 3) - pow(k, 2) + 2.0 / 3);
	else if (k >= 1 && k < 2) return (((-1.0 / 6) * pow(k, 3) + pow(k, 2) - 2 * k + 4 / 3.0));
	else return 0;
}

void interpolation_NN(Mat img_in, Mat& img_forward, int multiple) {
	// forward
	img_forward = Mat(img_in.rows * multiple, img_in.cols * multiple, CV_32F, Scalar(0));
	float d = 1 / (float)multiple;

	for (int j = 0; j < img_in.rows; j++) {
		for (int i = 0; i < img_in.cols; i++) {
			img_forward.at<float>(j * multiple, i * multiple) = img_in.at<uchar>(j, i);
		}
	}

	// x-axe
	for (int j = 0; j < img_in.rows; j++) {
		for (int i = 0; i < img_in.cols; i++) {
			for (int ky = 0; ky < multiple; ky++) {
				for (int kx = 0; kx < multiple; kx++) {
					img_forward.at<float>(j * multiple + ky, i * multiple + kx) = img_in.at<uchar>(j, i);
				}
			}
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

	interpolation_NN(img_in, img_out, multiple);

	cout << "width: " << img_out.cols << endl
		<< "height: " << img_out.rows << endl;

	imshow("original", img_in);
	imshow("NN interpolation", img_out);
	//imwrite("NN_interpolation.jpg", img_out);

	waitKey(0);
	return 0;
}