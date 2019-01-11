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
}

void erosion(Mat img_in, Mat& img_out, Mat mask) {
	Mat img_padding;
	zeroPad(img_in, img_padding, mask.rows);

	img_out = Mat(img_in.rows, img_in.cols, CV_32F);
	int center_mask = mask.rows / 2;

	for (int j = center_mask; j < img_padding.rows - center_mask; j++) {
		for (int i = center_mask; i < img_padding.cols - center_mask; i++) {
			int binary = 255;
			int flag = 0;
			for (int v = 0; v < mask.rows; v++) {
				for (int u = 0; u < mask.rows; u++) {
					if (img_padding.at<float>(j + v - center_mask, i + u - center_mask) != mask.at<float>(v, u)) {
						binary = 0;
						flag = 1;
						break;
					}
				}
				if (flag == 1) break;
			}
			img_out.at<float>(j - center_mask, i - center_mask) = binary;
		}
	}
}

void hitOrMiss(Mat img_in, Mat& img_out) {
	Mat img_complement(img_in.size(), CV_8U);
	for (int j = 0; j < img_in.rows; j++) {
		for (int i = 0; i < img_in.cols; i++) {
			if (img_in.at<uchar>(j, i) == 0) img_complement.at<uchar>(j, i) = 255;
			else img_complement.at<uchar>(j, i) = 0;
		}
	}

	float c[] = {
		255, 255, 255,
		0, 255, 0,
		0, 0, 0
	};
	float d[] = {
		0, 0, 0,
		0, 0, 0,
		255, 255, 255
	};
	Mat C(3, 3, CV_32F, c);
	Mat D(3, 3, CV_32F, d);
	Mat img_temp1, img_temp2;

	erosion(img_in, img_temp1, C);
	img_temp1.convertTo(img_temp1, CV_8U);
	imshow("erosion1", img_temp1);
	//imwrite("hitormiss_erosion1.jpg", img_temp1);

	erosion(img_complement, img_temp2, D);
	img_temp2.convertTo(img_temp2, CV_8U);
	imshow("erosion2", img_temp2);
	//imwrite("hitormiss_erosion2.jpg", img_temp2);

	for (int j = 0; j < img_temp1.rows; j++) {
		for (int i = 0; i < img_temp1.cols; i++) {
			img_out.at<uchar>(j, i) = img_temp1.at<uchar>(j, i) & img_temp2.at<uchar>(j, i);
		}
	}
}

int main(void)
{
	Mat img_gray = imread("img3.bmp", 0);
	if (img_gray.empty()) {
		cout << "image load failed!" << endl;
		return -1;
	}
	Mat img_hitormiss(img_gray.size(), CV_8U);

	hitOrMiss(img_gray, img_hitormiss);

	imshow("original", img_gray);
	imshow("hit or miss", img_hitormiss);
	imwrite("hit_or_miss.jpg", img_hitormiss);

	waitKey(0);
	return 0;
}