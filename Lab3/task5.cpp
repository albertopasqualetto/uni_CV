#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <iostream>
#include <cmath>

using namespace cv;

double euclideanDistance(Mat img, int row, int col, Vec<double, 3> avgVec){
	return sqrt(
				pow(img.at<Vec3b>(row, col)[0] - avgVec[0], 2) +
				pow(img.at<Vec3b>(row, col)[1] - avgVec[1], 2) +
				pow(img.at<Vec3b>(row, col)[2] - avgVec[2], 2)
	);
}

static void mouseCallbackSegment(int event, int x, int y, int flags, void* img){
	if (event == EVENT_LBUTTONDOWN){
		Mat image = *static_cast<Mat*>(img);

		// Conversion to HSV
		Mat imageHSV;
		cvtColor(image, imageHSV, COLOR_BGR2HSV);

		Vec3b colorHSV = imageHSV.at<Vec3b>(y, x);   // row, col
		std::cout << "This pixel HSV values: (" << (int)colorHSV[0] << ", " << (int)colorHSV[1] << ", " << (int)colorHSV[2] << ")" << std::endl;

		const int BORDER = 9;
		int radius = BORDER / 2;
		int sumH = 0;
		int sumS = 0;
		int sumV = 0;
		int pixelCount = 0;
		for(int j = -1 * radius; j <= radius; j++) {
			if (y + j < 0 || y + j >= image.rows)
				continue;
			for (int i = -1 * radius; i <= radius; i++) {
				if (x + i < 0 || x + i >= image.cols)
					continue;
				sumH += (int) imageHSV.at<Vec3b>(y + j, x + i)[0];
				sumS += (int) imageHSV.at<Vec3b>(y + j, x + i)[1];
				sumV += (int) imageHSV.at<Vec3b>(y + j, x + i)[2];
				pixelCount++;
			}
		}
		double avgH = (double) sumH / pixelCount;
		double avgS = (double) sumS / pixelCount;
		double avgV = (double) sumV / pixelCount;
		std::cout << "This " << BORDER << 'x' << BORDER << " neighborhood HSV values: (" << avgH << ", " << avgS << ", " << avgV << ")" << std::endl;


		// Segmentation
		const int T = 5;   // Threshold
		Mat mask = Mat::zeros(imageHSV.rows, imageHSV.cols, imageHSV.type()-16); // -16 means 3 channels to 1 channel
		for(int j=0; j < imageHSV.rows; j++){
			for(int i=0; i < imageHSV.cols; i++){
//				if(euclideanDistance(imageHSV, j, i, Vec<double, 3>(avgH, avgS, avgV)) < T){
//					mask.at<uchar>(j, i) = 255;
//				}
				if(abs(imageHSV.at<Vec3b>(j, i)[0] - avgH) < T){    // Hue distance (useful because we are interested in a specific color)
					mask.at<uchar>(j, i) = 255;
				}
			}
		}
		imshow("Masked Image", mask);
//		imwrite("mask.jpg", mask);
	}
}

int main(int argc, char** argv){
	std::string filename = "robocup.jpg";
	Mat img = imread(filename);
	if (img.data == NULL){
		std::cerr<<"The image cannot be read (because of missing file, improper permissions, unsupported or invalid format)"<<std::endl;
		return 1;
	}

	imshow("Original image", img);
	setMouseCallback("Original image", mouseCallbackSegment, &img);

	waitKey(0);

	return 0;
}