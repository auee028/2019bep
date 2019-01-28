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

void dilation(Mat img_in, Mat& img_out, Mat mask) {
	Mat img_padding;
	zeroPad(img_in, img_padding, mask.rows);

	img_out = Mat(img_in.rows, img_in.cols, CV_8U);
	int center_mask = mask.rows / 2;

	for (int j = center_mask; j < img_padding.rows - center_mask; j++) {
		for (int i = center_mask; i < img_padding.cols - center_mask; i++) {
			// when the different fixel is detected, output pixel becomes 1
			int binary = 0;
			int flag = 0;
			for (int v = 0; v < mask.rows; v++) {
				for (int u = 0; u < mask.rows; u++) {
					if (img_padding.at<uchar>(j + v - mask.rows / 2, i + u - mask.rows / 2) != mask.at<uchar>(v, u)) {
						binary = 255;
						flag = 1;
						break;
					}
				}
				if (flag == 1) break;
			}
			img_out.at<uchar>(j - center_mask, i - center_mask) = binary;
		}
	}
}

void graydilation(Mat img_in, Mat& img_out, Mat mask) {
	Mat img_padding;
	zeroPad(img_in, img_padding, mask.rows);

	img_out = Mat(img_in.rows, img_in.cols, CV_8U);
	int center_mask = mask.rows / 2;

	for (int j = center_mask; j < img_padding.rows - center_mask; j++) {
		for (int i = center_mask; i < img_padding.cols - center_mask; i++) {
			int max = 0;
			for (int v = 0; v < mask.rows; v++) {
				for (int u = 0; u < mask.rows; u++) {
					if (img_padding.at<uchar>(j + v - mask.rows / 2, i + u - mask.rows / 2) > max) {
						max = img_padding.at<uchar>(j + v - mask.rows / 2, i + u - mask.rows / 2);
					}
				}
			}
			img_out.at<uchar>(j - center_mask, i - center_mask) = max;
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
	Mat img_dilation(img_gray.size(), CV_8U);
	Mat img_graydilation(img_gray.size(), CV_8U);
	int data_mask[9] = { 0, };
	Mat mask(3, 3, CV_8U, data_mask);

	dilation(img_gray, img_dilation, mask);
	graydilation(img_gray, img_graydilation, mask);

	imshow("original", img_gray);
	//imshow("dilation", img_dilation);
	//imwrite("dilation_5.jpg", img_dilation);
	imshow("grayscale dilation", img_graydilation);
	imwrite("graydilation.jpg", img_graydilation);

	waitKey(0);
	return 0;
}