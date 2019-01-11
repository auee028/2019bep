#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

void zeroPad(Mat img_in, Mat& img_padding, int mask_size) {

	int height = img_in.rows;
	int width = img_in.cols;

	img_padding = Mat(height + (mask_size - 1), width + (mask_size - 1), CV_32F, Scalar(0));

	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			img_padding.at<float>(j + (mask_size - 1) / 2, i + (mask_size - 1) / 2) = img_in.at<uchar>(j, i);
		}
	}

	img_padding.convertTo(img_padding, CV_8U);
	imshow("zero padding", img_padding);

	waitKey(0);
}

void convolution(Mat img_in, Mat& img_out, Mat mask) {

	img_out = Mat(img_in.rows - (mask.rows - 1), img_in.cols - (mask.cols - 1), CV_32F, Scalar(0));
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
			img_out.at<float>(j - center_mask.y, i - center_mask.x) = sum;
		}
	}
}

void gaussian(Mat img_gray, Mat gaussian_mask, Mat& img_out, float sigma, int mask_size) {

	float ratio = 1 / (2 * CV_PI * powf(sigma, 2));

	float sum = 0;
	for (int j = -mask_size / 2; j < mask_size / 2 + 1; j++) {
		for (int i = -mask_size / 2; i < mask_size / 2 + 1; i++) {
			gaussian_mask.at<float>(j + mask_size / 2, i + mask_size / 2) = ratio * exp(-(pow(j, 2) + pow(i, 2)) / (2 * powf(sigma, 2)));
			sum += gaussian_mask.at<float>(j + mask_size / 2, i + mask_size / 2);
		}
	}
	cout << "mask_sum = " << sum << endl;

	convolution(img_gray, img_out, gaussian_mask);
	img_out.convertTo(img_out, CV_8U);

	imshow("original", img_gray);
	imshow("gaussian smoothing filtering", img_out);

	waitKey(0);
}

void laplacian_alldir(Mat img_in, Mat& img_out, int mask_size) {

	Mat mask_laplacian(mask_size, mask_size, CV_32F);
	for (int j = 0; j < mask_size; j++) {
		for (int i = 0; i < mask_size; i++) {
			mask_laplacian.at<float>(j, i) = 1.0;
		}
	}
	mask_laplacian.at<float>(mask_size / 2, mask_size / 2) = 1 - mask_size * mask_size;

	convolution(img_in, img_out, mask_laplacian);

	img_out.convertTo(img_out, CV_8U);

	waitKey(0);
};

void laplacianOfGaussian(Mat img_padding, Mat& img_out, float sigma, int mask_size) {	// LoG

	Mat mask_gaussian(mask_size, mask_size, CV_32F);
	Mat img_temp;

	gaussian(img_padding, mask_gaussian, img_out, sigma, mask_size);
	zeroPad(img_out, img_temp, mask_size);
	laplacian_alldir(img_temp, img_out, mask_size);

	imshow("laplacian of gaussian", img_out);
	imwrite("LoG_sigma0.4_alldir.jpg", img_out);

	waitKey(0);
}

int main(void) {
	Mat img_gray = imread("Woman.bmp", 0);
	if (img_gray.empty()) {
		cerr << "Image load failed!" << endl;
		return -1;
	}
	Mat img_padding;
	Mat img_out;

	float sigma = 0.4; // same as kernel size
	int mask_size = (int)(6.0 * sigma) + 1;

	zeroPad(img_gray, img_padding, mask_size);
	laplacianOfGaussian(img_padding, img_out, sigma, mask_size);

	return 0;
}