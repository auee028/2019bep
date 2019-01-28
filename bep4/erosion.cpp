# include <iostream>
# include <opencv2/opencv.hpp>
# include <opencv2/core.hpp>
# include <opencv2/highgui.hpp>

using namespace std;
using namespace cv;

void zeroPad(Mat img_in, Mat& img_padding, int mask_size) {

	int height = img_in.rows;
	int width = img_in.cols;

	img_padding = Mat(height + (mask_size - 1), width + (mask_size - 1), CV_8U, Scalar(0));

	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			img_padding.at<uchar>(j + (mask_size - 1) / 2, i + (mask_size - 1) / 2) = img_in.at<uchar>(j, i);
		}
	}
}

void erosion(Mat img_in, Mat& img_out, Mat mask) {
	Mat img_padding;
	zeroPad(img_in, img_padding, mask.rows);

	img_out = Mat(img_in.rows, img_in.cols, CV_8U);
	int center_mask = mask.rows / 2;

	for (int j = center_mask; j < img_padding.rows - center_mask; j++) {
		for (int i = center_mask; i < img_padding.cols - center_mask; i++) {
			// when the different fixel is detected, output pixel becomes 0
			int binary = 255;
			int flag = 0;
			for (int v = 0; v < mask.rows; v++) {
				for (int u = 0; u < mask.rows; u++) {
					if (img_padding.at<uchar>(j + v - mask.rows / 2, i + u - mask.rows / 2) != mask.at<float>(v, u)) {
						binary = 0;
						flag = 1;
						break;
					}
				}
				if (flag == 1) break;
			}
			img_out.at<uchar>(j - center_mask, i - center_mask) = binary;
		}
	}
	img_out.convertTo(img_out, CV_8U);
}

void grayerosion(Mat img_in, Mat& img_out, Mat mask) {
	Mat img_padding;
	zeroPad(img_in, img_padding, mask.rows);

	img_out = Mat(img_in.rows, img_in.cols, CV_8U);
	int center_mask = mask.rows / 2;

	for (int j = center_mask; j < img_padding.rows - center_mask; j++) {
		for (int i = center_mask; i < img_padding.cols - center_mask; i++) {
			int min = 300;
			for (int v = 0; v < mask.rows; v++) {
				for (int u = 0; u < mask.rows; u++) {
					if (img_padding.at<uchar>(j + v - mask.rows / 2, i + u - mask.rows / 2) < min) {
						min = img_padding.at<uchar>(j + v - mask.rows / 2, i + u - mask.rows / 2);
					}
				}
			}
			img_out.at<uchar>(j - center_mask, i - center_mask) = min;
		}
	}
}

int main(void)
{
	Mat img_gray = imread("LENA256.jpg", 0);
	if (img_gray.empty()) {
		cout << "image load failed!" << endl;
		return -1;
	}

	//for (int j = 0; j < img_gray.rows; j++) {
	//	for (int i = 0; i < img_gray.cols; i++) {
	//		if (img_gray.at<uchar>(j, i) > 127) img_gray.at<uchar>(j, i) = 255;
	//		else img_gray.at<uchar>(j, i) = 0;
	//	}
	//}

	Mat img_erosion(img_gray.size(), CV_8U);
	float data_mask[9] = {
		255, 255, 255,
		255, 255, 255,
		255, 255, 255
	};
	Mat mask(3, 3, CV_32F, data_mask);

	Mat img_grayerosion(img_gray.size(), CV_8U);
	float data_graymask[9] = { 0, };
	Mat graymask(3, 3, CV_32F, data_graymask);

	erosion(img_gray, img_erosion, mask);
	grayerosion(img_gray, img_grayerosion, graymask);

	imshow("original", img_gray);
	//imshow("erosion", img_erosion);
	//imwrite("erosion.jpg", img_erosion);
	imshow("grayscale erosion", img_grayerosion);
	imwrite("grayerosion.jpg", img_grayerosion);

	waitKey(0);
	return 0;
}