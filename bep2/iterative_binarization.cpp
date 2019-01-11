#include "opencv2/opencv.hpp"
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>
#include <ctime>

using namespace cv;
using namespace std;

int main(void)
{
	Mat img_gray = imread("LENA256.jpg", 0);	// LENA256.jpg is not a grayscale image(true color image) -> without second arg., only one third of the image is processed.
	if (img_gray.empty()) {
		cerr << "Image load failed!" << endl;
		return -1;
	}

	int width = img_gray.cols;
	int height = img_gray.rows;

	srand((unsigned int)time(0));		// seed for random value
	int threshold = rand() % 256;			// a~b: rand() % (b - a + 1) + a	(rand(): 0~32767)

											// dynamic binarization; iterative binarization
	Mat img_iterbin(height, width, CV_8UC1);
	int k1 = 0; int k2 = 0;
	int sum1 = 0; int sum2 = 0;
	int old_thres = 0;

	do {
		old_thres = threshold;

		for (int j = 0; j < height; j++) {	// pdf
			for (int i = 0; i < width; i++) {
				if (img_gray.at<uchar>(j, i) > threshold) {
					sum1 += img_gray.at<uchar>(j, i);
					k1++;
				}
				else {
					sum2 += img_gray.at<uchar>(j, i);
					k2++;
				}
			}
		}

		threshold = ((sum1 / k1) + (sum2 / k2)) / 2;
	} while (threshold != old_thres);

	for (int j = 0; j < 256; j++) {
		for (int i = 0; i < 256; i++) {
			if (img_gray.at<uchar>(j, i) > threshold) img_iterbin.at<uchar>(j, i) = 255;
			else img_iterbin.at<uchar>(j, i) = 0;
		}
	}

	imshow("original", img_gray);
	imshow("iterative binarization", img_iterbin);
	//imwrite("iterative_original.jpg", img_gray);
	//imwrite("iterative_binarization.jpg", img_iterbin);		// Don't miss .jpg

	waitKey(0);
	return 0;
}

