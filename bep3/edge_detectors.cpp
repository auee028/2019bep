#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

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

int main(void) {
	Mat img_gray = imread("Test1.bmp", 0);
	if (img_gray.empty()) {
		cerr << "Image load failed!" << endl;
		return -1;
	}

	int mask_size = 3;

	float data_sobel_x[] = {
		-1, 0, 1,
		-2, 0, 2,
		-1, 0, 1
	};

	float data_sobel_y[] = {
		-1, -2, -1,
		0, 0, 0,
		1, 2, 1
	};

	float data_prewitt_x[] = {
		-1, 0, 1,
		-1, 0, 1,
		-1, 0, 1
	};

	float data_prewitt_y[] = {
		-1, -1, -1,
		0, 0, 0,
		1, 1, 1
	};

	float data_roberts_ur[] = {
		-1, 0, 0,
		0, 1, 0,
		0, 0, 0
	};

	float data_roberts_ul[] = {
		0, 0, -1,
		0, 1, 0,
		0, 0, 0
	};

	float data_laplacian_xydir[] = {
		0, 1, 0,
		1, -4, 1,
		0, 1, 0
	};

	float data_laplacian_alldir[] = {
		1, 1, 1,
		1, -8, 1,
		1, 1, 1
	};

	Mat edge_detection(mask_size, mask_size, CV_32F, data_laplacian_alldir);
	Mat img_out;
	convolution(img_gray, img_out, edge_detection);
	img_out.convertTo(img_out, CV_8U);

	imshow("original", img_gray);
	imshow("edge detection", img_out);
	imwrite("laplacian_alldir_edge_detection.jpg", img_out);

	waitKey(0);
	return 0;
}