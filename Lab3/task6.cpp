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

		Vec3b colorBGR = image.at<Vec3b>(y, x);   // row, col
		std::cout<<"This pixel BGR values: ("<<(int)colorBGR[0]<<", "<<(int)colorBGR[1]<<", "<<(int)colorBGR[2]<<")"<<std::endl;

		const int BORDER = 9;
		int radius = BORDER / 2;
		int sumB = 0;
		int sumG = 0;
		int sumR = 0;
		int pixelCount = 0;
		for(int j = -1 * radius; j <= radius; j++) {
			if (y + j < 0 || y + j >= image.rows)
				continue;
			for (int i = -1 * radius; i <= radius; i++) {
				if (x + i < 0 || x + i >= image.cols)
					continue;
				sumB += (int) image.at<Vec3b>(y+j, x+i)[0];
				sumG += (int) image.at<Vec3b>(y+j, x+i)[1];
				sumR += (int) image.at<Vec3b>(y+j, x+i)[2];
				pixelCount++;
			}
		}
		double avgB = (double) sumB / pixelCount;
		double avgG = (double) sumG / pixelCount;
		double avgR = (double) sumR / pixelCount;
		std::cout << "This " << BORDER << 'x' << BORDER << " neighborhood BGR values: (" << avgB << ", " << avgG << ", " << avgR << ")" << std::endl;


		// Segmentation
		const int T = 75;   // Threshold
		Mat mask = Mat::zeros(image.rows, image.cols, image.type()-16); // -16 means 3 channels to 1 channel
		for(int j=0; j < image.rows; j++){
			for(int i=0; i < image.cols; i++){
				if(euclideanDistance(image, j, i, Vec<double, 3>(avgB, avgG, avgR)) < T){
					mask.at<uchar>(j, i) = 255;
				}
			}
		}
		imshow("Masked Image", mask);


		//Recolor
		const Vec3b newColor = Vec3b(92, 37, 201);
		Mat recoloredImage = image.clone();
		for(int j=0; j < image.rows; j++){
			for(int i=0; i < image.cols; i++){
				if(euclideanDistance(image, j, i, Vec<double, 3>(avgB, avgG, avgR)) < T){
					recoloredImage.at<Vec3b>(j, i) = newColor;
				}
			}
		}
		imshow("Recolored Image", recoloredImage);
		imwrite("recolor.jpg", recoloredImage);
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