#include "opencv2/opencv.hpp"
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>

using namespace cv;
using namespace std;

int main(void)
{
	Mat img_input = imread("lena.jpg");
	if (img_input.empty()) {
		cerr << "Image load failed!" << endl;
		return -1;
	}

	int width = img_input.cols;
	int height = img_input.rows;

	//cout << "width: " << width << ", height: " << height << endl;	// print

	imshow("original", img_input);

	// grayscale
	Mat img_out1(img_input.rows, img_input.cols, CV_8UC1);	// size, unsigned char 1 channel
	int b, g, r, gray = 0;

	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			b = img_input.at<Vec3b>(y, x)[0];
			g = img_input.at<Vec3b>(y, x)[1];
			r = img_input.at<Vec3b>(y, x)[2];
			gray = (r + g + b) / 3;
			img_out1.at<uchar>(y, x) = gray;
		}
	}

	imshow("grayscale", img_out1);

	// Real RGBY
	Mat img_out2(img_input.rows, img_input.cols, CV_8UC4);
	double R, G, B, Y;

	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			b = img_input.at<Vec3b>(y, x)[0];	// vector with 3 byte entries
			g = img_input.at<Vec3b>(y, x)[1];
			r = img_input.at<Vec3b>(y, x)[2];

			R = r - (g + b) / 2.0;
			G = g - (r + b) / 2.0;
			B = b - (r + g) / 2.0;
			Y = (r + g) / 2.0 - abs(r - g) / 2.0 - b;

			if (R < 0) R = 0;
			if (G < 0) G = 0;
			if (B < 0) B = 0;
			if (Y < 0) Y = 0;

			img_out2.at<Vec4b>(y, x)[0] = (int)R;
			img_out2.at<Vec4b>(y, x)[1] = (int)G;
			img_out2.at<Vec4b>(y, x)[2] = (int)B;
			img_out2.at<Vec4b>(y, x)[3] = (int)Y;
		}
	}
	imshow("Real RGBY", img_out2);

	// CMYK
	Mat img_out3(img_input.rows, img_input.cols, CV_8UC4);
	double Rc, Gc, Bc;
	double k, c, m, y;

	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			b = img_input.at<Vec3b>(j, i)[0];	// vector with 3 byte entries
			g = img_input.at<Vec3b>(j, i)[1];
			r = img_input.at<Vec3b>(j, i)[2];

			Rc = r / 255.0;
			Gc = g / 255.0;
			Bc = b / 255.0;

			k = 1 - max(max(Rc, Gc), Bc);		// black key color
			if (k != 1) {
				c = (1 - Rc - k) / (1 - k);		// cyan
				m = (1 - Gc - k) / (1 - k);		// magenta
				y = (1 - Bc - k) / (1 - k);		// yellow
			}
			else if (k = 1) c = 0;
			else if (k = 1) m = 0;
			else if (k = 1) y = 0;

			img_out3.at<Vec4b>(j, i)[0] = (int)(k * 255);
			img_out3.at<Vec4b>(j, i)[1] = (int)(y * 255);
			img_out3.at<Vec4b>(j, i)[2] = (int)(m * 255);
			img_out3.at<Vec4b>(j, i)[3] = (int)(c * 255);
		}
	}
	imshow("RGB2CMYK", img_out3);

	// HSV
	Mat img_out4(img_input.rows, img_input.cols, CV_8UC3);
	double Cmax, Cmin, del;
	double H, S, V;

	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			b = img_input.at<Vec3b>(y, x)[0];	// vector with 3 byte entries
			g = img_input.at<Vec3b>(y, x)[1];
			r = img_input.at<Vec3b>(y, x)[2];

			Rc = r / 255.0;
			Gc = g / 255.0;
			Bc = b / 255.0;

			Cmax = max(max(Rc, Gc), Bc);
			Cmin = min(min(Rc, Gc), Bc);

			V = Cmax;

			double theta = acos(((Rc - Gc) + (Rc - Bc)) / 2 / sqrt((Rc - Gc)*(Rc - Gc) + (Rc - Bc)*(Gc - Bc)));
			if (Bc >= Gc) H = 360 - theta;
			else H = theta;

			S = 1 - 3 / (Rc + Gc + Bc) * Cmin;

			V = (Rc + Gc + Bc) / 3;

			img_out4.at<Vec3b>(y, x)[0] = (int)H;	// vector with 3 byte entries
			img_out4.at<Vec3b>(y, x)[1] = (int)(S * 255);
			img_out4.at<Vec3b>(y, x)[2] = (int)(V * 255);
		}
	}
	imshow("RGB2HSV", img_out4);

	waitKey(0);
	return 0;
}

