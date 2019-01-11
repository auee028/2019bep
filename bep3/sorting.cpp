#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <ctime>

using namespace std;
using namespace cv;

void selectionSort(int arr[], int size)
{
	int i, j, index, temp;

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

	cout << "result: " << endl;
	for (i = 0; i < size; i++) {
		cout << arr[i] << " ";
	}
	cout << endl;
}

void quickSort(int arr[], int left, int right) {	//recursion
	int i = left, j = right;
	int pivot = (left + right) / 2;
	int temp;

	do {
		while (arr[i] < arr[pivot]) i++;
		while (arr[j] > arr[pivot]) j--;
		if (i <= j) {
			temp = arr[i];
			arr[i] = arr[j];
			arr[j] = temp;
			i++;
			j--;
		}
	} while (i <= j);

	if (left < j) quickSort(arr, left, j);
	if (right > i) quickSort(arr, i, right);

}

int main(void) {
	int arr[10] = { 0, };
	int *list = arr;		// call of reference

	srand((unsigned)time(0));
	cout << "random array: " << endl;
	for (int i = 0; i < 10; i++) {
		arr[i] = rand() % 10;
		cout << arr[i] << " ";
	}
	cout << endl << endl;

	int size = sizeof(arr) / sizeof(int);
	//selectionSort(arr, size);
	quickSort(arr, 0, size - 1);
	cout << "result: " << endl;
	for (int i = 0; i < 10; i++) {
		cout << arr[i] << " ";
	}
	cout << endl;

	return 0;
}