#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

void zeroPad(Mat img_in, Mat& img_padding, int mask_size) {

	int height = img_in.rows;
	int width = img_in.cols;

	img_padding = Mat(height + (mask_size - 1), width + (mask_size - 1), CV_8UC1, Scalar(0));

	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			img_padding.at<uchar>(j + (mask_size - 1) / 2, i + (mask_size - 1) / 2) = img_in.at<uchar>(j, i);
		}
	}

	imshow("zero padding", img_padding);

	waitKey(0);
}

void selectionSort(int arr[], int size)
{
	int i, j, index;
	float temp;

	for (i = 0; i < size - 1; i++)
	{
		index = i;
		for (j = index + 1; j < size; j++) {
			if (arr[index] > arr[j]) index = j;
		}

		temp = arr[i];
		arr[i] = arr[index];
		arr[index] = temp;
	}
}

void medianFilter(Mat img_in, Mat& img_out, int mask_size) {
	int arr[9] = { 0, };
	int half = mask_size / 2;

	for (int j = half; j < img_in.rows - half; j++) {
		for (int i = half; i < img_in.cols - half; i++) {
			int count = 0;
			for (int v = 0; v < mask_size; v++) {
				for (int u = 0; u < mask_size; u++) {
					int y = j + v - half;
					int x = i + u - half;
					arr[count] = img_in.at<uchar>(y, x);
					count++;
				}
			}
			selectionSort(arr, mask_size * mask_size);
			img_out.at<uchar>(j - half, i - half) = (int)(arr[mask_size * mask_size / 2]);	// // After filtering, image size is reduced. Without initializing, trash values are remained at unprocessed pixel and it makes an '.exe' error.
		}
	}
}

void maximumFilter(Mat img_in, Mat& img_out, int mask_size) {
	int arr[9] = { 0, };
	int half = mask_size / 2;

	for (int j = half; j < img_in.rows - half; j++) {
		for (int i = half; i < img_in.cols - half; i++) {
			int count = 0;
			for (int v = 0; v < mask_size; v++) {
				for (int u = 0; u < mask_size; u++) {
					int y = j + v - half;
					int x = i + u - half;
					arr[count] = img_in.at<uchar>(y, x);
					count++;
				}
			}
			selectionSort(arr, mask_size * mask_size);
			img_out.at<uchar>(j - half, i - half) = (int)(arr[mask_size * mask_size - 1]);	// After filtering, image size is reduced. Without initializing, trash values are remained at unprocessed pixel and it makes an '.exe' error.
		}
	}
}

void minimumFilter(Mat img_in, Mat& img_out, int mask_size) {
	int arr[9] = { 0, };
	int half = mask_size / 2;

	for (int j = half; j < img_in.rows - half; j++) {
		for (int i = half; i < img_in.cols - half; i++) {
			int count = 0;
			for (int v = 0; v < mask_size; v++) {
				for (int u = 0; u < mask_size; u++) {
					int y = j + v - half;
					int x = i + u - half;
					arr[count] = img_in.at<uchar>(y, x);
					count++;
				}
			}
			selectionSort(arr, mask_size * mask_size);
			img_out.at<uchar>(j - half, i - half) = (int)(arr[0]);	// After filtering, image size is reduced. Without initializing, trash values are remained at unprocessed pixel and it makes an '.exe' error.
		}
	}
}

void midpointFilter(Mat img_in, Mat& img_out, int mask_size) {
	int arr[9] = { 0, };
	int half = mask_size / 2;
	int avg = 0;

	for (int j = half; j < img_in.rows - half; j++) {
		for (int i = half; i < img_in.cols - half; i++) {
			int count = 0;
			for (int v = 0; v < mask_size; v++) {
				for (int u = 0; u < mask_size; u++) {
					int y = j + v - half;
					int x = i + u - half;
					arr[count] = img_in.at<uchar>(y, x);
					count++;
				}
			}
			selectionSort(arr, mask_size * mask_size);
			avg = (arr[0] + arr[mask_size * mask_size - 1]) / 2;
			img_out.at<uchar>(j - half, i - half) = (int)(avg);	// After filtering, image size is reduced. Without initializing, trash values are remained at unprocessed pixel and it makes an '.exe' error.
		}
	}
}

void alphaTrimmedMeanFilter(Mat img_in, Mat& img_out, int mask_size) {
	int arr[9] = { 0, };
	int half = mask_size / 2;
	int alpha = 1;
	int avg = 0;

	for (int j = half; j < img_in.rows - half; j++) {
		for (int i = half; i < img_in.cols - half; i++) {
			int count = 0;
			for (int v = 0; v < mask_size; v++) {
				for (int u = 0; u < mask_size; u++) {
					int y = j + v - half;
					int x = i + u - half;
					arr[count] = img_in.at<uchar>(y, x);
					count++;
				}
			}
			selectionSort(arr, mask_size * mask_size);
			avg = (arr[alpha] + arr[mask_size * mask_size - 1 - alpha]) / 2;
			img_out.at<uchar>(j - half, i - half) = (int)(avg);	// After filtering, image size is reduced. Without initializing, trash values are remained at unprocessed pixel and it makes an '.exe' error.
		}
	}
}

int main(void) {
	int mask_size = 3;

	Mat img_gray = imread("Test1.bmp", 0);
	if (img_gray.empty()) {
		cerr << "Image load failed!" << endl;
		return -1;
	}
	Mat img_out;
	Mat img_result(img_gray.size(), CV_8U);

	zeroPad(img_gray, img_out, mask_size);

	//medianFilter(img_out, img_result, mask_size);
	//maximumFilter(img_out, img_result, mask_size);
	//minimumFilter(img_out, img_result, mask_size);
	//midpointFilter(img_out, img_result, mask_size);
	alphaTrimmedMeanFilter(img_out, img_result, mask_size);

	imshow("original", img_gray);
	imshow("filter", img_result);
	imwrite("alpha_trimmed_mean_filter_3.jpg", img_result);

	waitKey(0);
	return 0;
}