#include "opencv2/opencv.hpp"
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main(void)
{
	Mat img_input = imread("eye.jpg");
	if (img_input.empty()) {
		cerr << "Image load failed!" << endl;
		return -1;
	}

	circle(img_input, Point(72, 75), 22, Scalar(255, 255, 255), 2.5);
	line(img_input, Point(52, 75), Point(95, 75), Scalar(255, 255, 255), 2);
	line(img_input, Point(73, 54), Point(73, 95), Scalar(255, 255, 255), 2);

	imshow("original", img_input);
	imwrite("eye_draw.jpg", img_input);

	waitKey(0);
	return 0;
}
