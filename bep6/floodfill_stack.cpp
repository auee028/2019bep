#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <stack>

using namespace std;
using namespace cv;

void floodfill(Mat img_in) {
	stack<int> s;
	int label = 1;

	for (int j = 0; j < img_in.rows; j++) {
		for (int i = 0; i < img_in.cols; i++) {
			if (img_in.at<uchar>(j, i) != 255) continue;

			int x_arr[4] = { 1, 0, -1, 0 };
			int y_arr[4] = { 0, 1, 0, -1 };
			int u = i, v = j;
			s.push(u);
			s.push(v);

			do {
				if (img_in.at<uchar>(v, u) == 255)
					img_in.at<uchar>(v, u) = label * 50;

				int exist_neighbor = 0, nu = 0, nv = 0;
				for (int k = 0; k < 4; k++) {
					nu = u + x_arr[k];
					nv = v + y_arr[k];
					if (img_in.at<uchar>(nv, nu) == 255) {
						s.push(nu);
						s.push(nv);

						u = nu;
						v = nv;

						exist_neighbor = 1;
						break;
					}
				}
				if (exist_neighbor == 0) {
					v = s.top(); s.pop();
					u = s.top(); s.pop();
				}
			} while (!s.empty());
			label++;
		}
	}
	cout << label - 1 << endl;
}

int main(void) {
	Mat img_in = imread("test_label.bmp", 0);
	if (img_in.empty()) {
		cout << "Image load failed!" << endl;
		return -1;
	}
	Mat img_labeling = img_in.clone();
	for (int j = 0; j < img_in.rows; j++) {
		for (int i = 0; i < img_in.cols; i++) {
			if (img_labeling.at<uchar>(j, i) > 125) img_labeling.at<uchar>(j, i) = 255;
			else img_labeling.at<uchar>(j, i) = 0;
		}
	}

	floodfill(img_labeling);

	Mat img_color(img_in.rows, img_in.cols, CV_8UC3, Scalar(0, 0, 0));
	for (int j = 0; j < img_in.rows; j++) {
		for (int i = 0; i < img_in.cols; i++) {
			if (img_labeling.at<uchar>(j, i) != 0 && img_labeling.at<uchar>(j, i) % 3 == 1) {
				img_color.at<Vec3b>(j, i)[2] = 125;
				img_color.at<Vec3b>(j, i)[1] = img_labeling.at<uchar>(j, i) * 50;
			}
			else if (img_labeling.at<uchar>(j, i) != 0 && img_labeling.at<uchar>(j, i) % 3 == 2) {
				img_color.at<Vec3b>(j, i)[1] = 125 + img_labeling.at<uchar>(j, i) * 10;
				img_color.at<Vec3b>(j, i)[0] = img_labeling.at<uchar>(j, i) * 50;
			}
			if (img_labeling.at<uchar>(j, i) != 0 && img_labeling.at<uchar>(j, i) % 3 == 0) {
				img_color.at<Vec3b>(j, i)[0] = 125 + img_labeling.at<uchar>(j, i) * 10;
				img_color.at<Vec3b>(j, i)[2] = img_labeling.at<uchar>(j, i) * 50;
			}
		}
	}

	imshow("original", img_in);
	imshow("labeling", img_labeling);
	imshow("labeling_color", img_color);
	imwrite("floodrill_gray.jpg", img_labeling);
	imwrite("floodfill_color.jpg", img_color);

	waitKey(0);
	return 0;
}
