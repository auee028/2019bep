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

void highboostSpatial(Mat img_in, Mat& img_out, int mask_size, float weight) {
	Mat allpass(mask_size, mask_size, CV_32F, Scalar(0));
	allpass.at<float>(mask_size / 2, mask_size / 2) = 1.0;
	Mat highpass(mask_size, mask_size, CV_32F, Scalar(0));
	for (int i = 0; i < mask_size; i++)
		highpass.at<float>(mask_size / 2, i) = -1.0 * weight;
	for (int j = 0; j < mask_size; j++)
		highpass.at<float>(j, mask_size / 2) = -1.0 * weight;
	highpass.at<float>(mask_size / 2, mask_size / 2) = 4 * (mask_size / 2) * weight;

	Mat img_padding1, img_padding2;
	Mat img_temp1, img_temp2;

	zeroPad(img_in, img_padding1, mask_size);
	convolution(img_padding1, img_temp1, allpass);

	zeroPad(img_in, img_padding2, mask_size);
	convolution(img_padding2, img_temp2, highpass);

	img_out = Mat(img_temp1.size(), CV_32F);
	for (int j = 0; j < img_temp1.rows; j++) {
		for (int i = 0; i < img_temp1.cols; i++) {
			img_out.at<float>(j, i) = img_temp1.at<float>(j, i) + img_temp2.at<float>(j, i);
		}
	}
	img_out.convertTo(img_out, CV_8U);
}

int main(void)
{
	Mat img_gray = imread("cat.jpg", 0);
	if (img_gray.empty()) {
		cout << "image load failed!" << endl;
		return -1;
	}
	Mat img_out;

	int mask_size = 3;
	float weight = 0.3;

	highboostSpatial(img_gray, img_out, mask_size, weight);

	imshow("original", img_gray);
	imshow("highboost spatial", img_out);
	imwrite("highboost_spatial.jpg", img_out);

	waitKey(0);
	return 0;
}