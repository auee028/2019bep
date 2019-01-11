#include "opencv2/opencv.hpp"
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main(void)
{
	Mat img_input = imread("lena.jpg");

	Mat img_out;
	if (img_input.empty()) {
		cerr << "Image load failed!" << endl;
		return -1;
	}

	cvtColor(img_input, img_out, COLOR_RGB2HSV);

	imshow("original", img_input);
	imshow("cvtColor_rgb2hsv", img_out);
	imwrite("cvtColor_rgb2hsv.jpg", img_out);

	waitKey(0);
	return 0;
}
