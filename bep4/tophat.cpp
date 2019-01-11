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

void dilation(Mat img_in, Mat& img_out, int mask_size) {
	Mat img_padding;
	zeroPad(img_in, img_padding, mask_size);

	img_out = Mat(img_in.rows, img_in.cols, CV_8U);
	int center_mask = mask_size / 2;

	for (int j = center_mask; j < img_padding.rows - center_mask; j++) {
		for (int i = center_mask; i < img_padding.cols - center_mask; i++) {
			// when 1 is detected, output pixel changes to 1
			int binary = 0;
			int flag = 0;
			for (int v = 0; v < mask_size; v++) {
				for (int u = 0; u < mask_size; u++) {
					if (img_padding.at<uchar>(j + v - mask_size / 2, i + u - mask_size / 2) != 0) {
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

	imshow("opening_dilation", img_out);
	imwrite("opening_dilation.jpg", img_out);
}

void erosion(Mat img_in, Mat& img_out, int mask_size) {
	Mat img_padding;
	zeroPad(img_in, img_padding, mask_size);

	img_out = Mat(img_in.rows, img_in.cols, CV_8U);
	int center_mask = mask_size / 2;

	for (int j = center_mask; j < img_padding.rows - center_mask; j++) {
		for (int i = center_mask; i < img_padding.cols - center_mask; i++) {
			// when 0 is detected, output pixel changes to 0
			int binary = 255;
			int flag = 0;
			for (int v = 0; v < mask_size; v++) {
				for (int u = 0; u < mask_size; u++) {
					if (img_padding.at<uchar>(j + v - mask_size / 2, i + u - mask_size / 2) == 0) {
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

	imshow("opening_erosion", img_out);
	imwrite("opening_erosion.jpg", img_out);
}

void opening(Mat img_in, Mat& img_out, int mask_size) {									// erosion -> dilation

	erosion(img_in, img_out, mask_size);
	dilation(img_out, img_out, mask_size);
}

void tophat(Mat img_original, Mat img_out, int mask_size) {		// original - opening
	Mat img_opening;
	opening(img_original, img_opening, mask_size);

	for (int j = 0; j < img_opening.rows; j++) {
		for (int i = 0; i < img_opening.cols; i++) {
			img_out.at<uchar>(j, i) = img_original.at<uchar>(j, i) - img_opening.at<uchar>(j, i);
		}
	}
}

int main(void)
{
	Mat img_gray = imread("noise_image.bmp", 0);
	if (img_gray.empty()) {
		cout << "image load failed!" << endl;
		return -1;
	}
	Mat img_tophat(img_gray.size(), CV_8U);

	int mask_size = 3;

	tophat(img_gray, img_tophat, mask_size);

	imshow("original", img_gray);
	imshow("tophat", img_tophat);
	imwrite("tophat.jpg", img_tophat);

	waitKey(0);
	return 0;
}