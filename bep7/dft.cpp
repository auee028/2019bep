#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

using namespace std;
using namespace cv;

void twodimDFT(Mat img_in, Mat& freq_real, Mat& freq_imag, Mat& img_out) {
	Mat img_real = img_in.clone();
	img_real.convertTo(img_real, CV_64F);
	Mat img_imag(img_in.size(), CV_64F, Scalar(0));
	Mat temp_real(img_real.size(), CV_64F, Scalar(0));
	Mat temp_imag(img_imag.size(), CV_64F, Scalar(0));

	int W = img_in.cols;
	int H = img_in.rows;

	// x-axis
	for (int j = 0; j < H; j++) {
		for (int k = 0; k < W; k++) {
			double sum_real = 0, sum_imag = 0;
			for (int n = 0; n < W; n++) {
				double theta = 2 * CV_PI / W * k * n;
				sum_real += img_real.at<double>(j, n) * cos(theta) + img_imag.at<double>(j, n) * sin(theta);
				sum_imag += img_imag.at<double>(j, n) * cos(theta) - img_real.at<double>(j, n) * sin(theta);
			}
			temp_real.at<double>(j, k) = sum_real;
			temp_imag.at<double>(j, k) = sum_imag;
		}
	}


	// y-axis
	for (int i = 0; i < W; i++) {
		for (int k = 0; k < H; k++) {
			double sum_real = 0, sum_imag = 0;
			for (int n = 0; n < H; n++) {
				double theta = 2 * CV_PI / H * k * n;
				sum_real += temp_real.at<double>(n, i) * cos(theta) + temp_imag.at<double>(n, i) * sin(theta);
				sum_imag += temp_imag.at<double>(n, i) * cos(theta) - temp_real.at<double>(n, i) * sin(theta);
			}
			freq_real.at<double>(k, i) = sum_real;
			freq_imag.at<double>(k, i) = sum_imag;
		}
	}

	// spectrum
	double spec = 0;
	Mat img_spectrum(img_in.size(), CV_64F, Scalar(0));
	double dc = sqrt(pow(freq_real.at<double>(0, 0), 2) + pow(freq_imag.at<double>(0, 0), 2));
	dc = 10 * log(dc + 1);
	for (int j = 0; j < H; j++) {
		for (int i = 0; i < W; i++) {
			spec = sqrt(pow(freq_real.at<double>(j, i), 2) + pow(freq_imag.at<double>(j, i), 2));
			img_spectrum.at<double>(j, i) = 10 * log(spec + 1) / dc * 255;
		}
	}

	// shift
	for (int j = 0; j < H / 2; j++) {   // even number of height
		for (int i = 0; i < W / 2; i++) {
			img_out.at<double>(j + H / 2, i + W / 2) = img_spectrum.at<double>(j, i);
			img_out.at<double>(j, i) = img_spectrum.at<double>(j + H / 2, i + W / 2);
			img_out.at<double>(j, i + W / 2) = img_spectrum.at<double>(j + H / 2, i);
			img_out.at<double>(j + H / 2, i) = img_spectrum.at<double>(j, i + W / 2);
		}
	}
}

void twodimIDFT(Mat freq_real, Mat freq_imag, Mat& img_out) {
	Mat temp_real(freq_real.size(), CV_64F);
	Mat temp_imag(freq_imag.size(), CV_64F);
	Mat time_real(freq_real.size(), CV_64F);
	Mat time_imag(freq_imag.size(), CV_64F);

	int W = freq_real.cols;
	int H = freq_real.rows;

	// x-axis
	for (int j = 0; j < H; j++) {
		for (int k = 0; k < W; k++) {
			double sum_real = 0, sum_imag = 0;
			for (int n = 0; n < W; n++) {
				double theta = 2 * CV_PI / W * k * n;
				sum_real += freq_real.at<double>(j, n) * cos(theta) - freq_imag.at<double>(j, n) * sin(theta);
				sum_imag += freq_imag.at<double>(j, n) * cos(theta) + freq_real.at<double>(j, n) * sin(theta);
			}
			sum_real /= W;
			sum_imag /= W;

			temp_real.at<double>(j, k) = sum_real;
			temp_imag.at<double>(j, k) = sum_imag;
		}
	}


	// y-axis
	for (int i = 0; i < W; i++) {
		for (int k = 0; k < H; k++) {
			double sum_real = 0, sum_imag = 0;
			for (int n = 0; n < H; n++) {
				double theta = 2 * CV_PI / H * k * n;
				sum_real += temp_real.at<double>(n, i) * cos(theta) - temp_imag.at<double>(n, i) * sin(theta);
				sum_imag += temp_imag.at<double>(n, i) * cos(theta) + temp_real.at<double>(n, i) * sin(theta);
			}
			sum_real /= H;
			sum_imag /= H;

			time_real.at<double>(k, i) = sum_real;
			time_imag.at<double>(k, i) = sum_imag;
		}
	}

	// spectrum
	double spec = 0;
	for (int j = 0; j < H; j++) {
		for (int i = 0; i < W; i++) {
			spec = sqrt(pow(time_real.at<double>(j, i), 2) + pow(time_imag.at<double>(j, i), 2));
			img_out.at<double>(j, i) = spec;
		}
	}


}

int main(void) {
	Mat img_in = imread("bird.png", 0);
	if (img_in.empty()) {
		cout << "Image load failed!" << endl;
		return -1;
	}
	// moire pattern
	for (int j = 0; j < img_in.rows; j++) {
		for (int i = 0; i < img_in.cols; i++) {
			if (j % (img_in.rows / 30) == 0 && j < img_in.rows - img_in.rows / 30 / 2) img_in.at<uchar>(j, i) = 0;
		}
	}

	Mat img_dft(img_in.size(), CV_64F, Scalar(0));
	Mat freq_real(img_in.size(), CV_64F, Scalar(0));
	Mat freq_imag(img_in.size(), CV_64F, Scalar(0));

	Mat img_idft(img_in.size(), CV_64F, Scalar(0));

	twodimDFT(img_in, freq_real, freq_imag, img_dft);

	int y[6] = { 16, 48, 79 , 142, 172, 205 };
	for (int j = 0; j < 6; j++) {
		for (int v = -3; v <= 3; v++) {
			for (int u = -5; u <= 5; u++) {
				freq_real.at<double>(y[j] + v, 110 + u) = 0;
				freq_imag.at<double>(y[j] + v, 110 + u) = 0;
				img_dft.at<double>(y[j] + v, 110 + u) = 0;
			}
		}
	}

	twodimIDFT(freq_real, freq_imag, img_idft);

	Mat img_showDFT = img_dft.clone();
	img_showDFT.convertTo(img_showDFT, CV_8U);
	Mat img_showIDFT = img_idft.clone();
	img_showIDFT.convertTo(img_showIDFT, CV_8U);

	imshow("original", img_in);
	imwrite("noise.jpg", img_in);
	imshow("dft", img_showDFT);
	imwrite("2D_DFT.jpg", img_showDFT);
	imshow("idft", img_showIDFT);
	imwrite("2D_IDFT.jpg", img_showIDFT);

	waitKey(0);
	return 0;
}