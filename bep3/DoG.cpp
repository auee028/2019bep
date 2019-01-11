# include <iostream>
# include <opencv2/opencv.hpp>
# include <opencv2/core.hpp>
# include <opencv2/highgui.hpp>

using namespace std;
using namespace cv;

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

void gaussianSmoothing(float sigma, int mask_size, Mat img_in, Mat& mask, Mat& img_out) {
	for (int j = 0; j < mask_size; j++) {
		for (int i = 0; i < mask_size; i++) {
			mask.at<float>(j, i) = (1.0 / (2.0 * CV_PI * sigma * sigma)) * exp(-(powf(j - mask_size / 2, 2) + powf(i - mask_size / 2, 2)) / (2 * sigma * sigma));
		}
	}

	convolution(img_in, img_out, mask);
}

void DoG(Mat img_gray, Mat& img_out) {			// DoG = gaussian_0.4 - gaussian_0.8

	Mat img_padding1, img_padding2;
	Mat img_temp1, img_temp2;

	float sigma1 = 0.4, sigma2 = 0.8;
	int mask_size1 = (int)(6.0 * sigma1) + 1;
	int mask_size2 = (int)(6.0 * sigma2) + 1;

	Mat mask_gaussian1(mask_size1, mask_size1, CV_32F);
	Mat mask_gaussian2(mask_size2, mask_size2, CV_32F);

	zeroPad(img_gray, img_padding1, mask_size1);
	gaussianSmoothing(sigma1, mask_size1, img_padding1, mask_gaussian1, img_temp1);

	zeroPad(img_gray, img_padding2, mask_size2);
	gaussianSmoothing(sigma2, mask_size2, img_padding2, mask_gaussian2, img_temp2);

	img_out = Mat(img_temp1.size(), CV_32F);
	for (int j = 0; j < img_temp1.rows; j++) {
		for (int i = 0; i < img_temp1.cols; i++) {
			img_out.at<float>(j, i) = img_temp1.at<float>(j, i) - img_temp2.at<float>(j, i);
		}
	}

	img_out.convertTo(img_out, CV_8U);
}

int main(void)
{
	Mat img_gray = imread("Woman.bmp", 0);
	Mat img_out;

	DoG(img_gray, img_out);

	imshow("original", img_gray);
	imshow("DoG", img_out);
	imwrite("DoG.jpg", img_out);

	waitKey(0);
	return 0;
}