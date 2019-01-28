#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

using namespace std;
using namespace cv;

void zoomOut(Mat img_in, Mat& img_forward, int ratio) {
	img_forward = Mat(img_in.rows / ratio, img_in.cols / ratio, CV_8U, Scalar(0));

	for (int j = 0; j < img_forward.rows; j++) {
		for (int i = 0; i < img_forward.cols; i++) {
			img_forward.at<uchar>(j, i) = img_in.at<uchar>(j / ratio, i / ratio);
		}
	}
}

int main(void) {
	Mat img_in = imread("small.bmp", 0);
	if (img_in.empty()) {
		cout << "Image load failed!" << endl;
		return -1;
	}
	Mat img_out;

	int ratio = 10;

	zoomOut(img_in, img_out, ratio);

	cout << "width: " << img_out.cols << endl
		<< "height: " << img_out.rows << endl;

	imshow("original", img_in);
	imshow("zoom out", img_out);
	imwrite("zoomout.jpg", img_out);

	waitKey(0);
	return 0;
}