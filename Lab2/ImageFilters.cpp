#include <opencv2/core.hpp>
#include "ImageFilters.h"
#include <iostream>
#include <limits>
#define UCHAR_MIN 0
// Suppose only unsigned chars and single channel

using namespace cv;

Mat minFilter(const cv::Mat& img, int kernelSize){
	if (kernelSize % 2 == 0){
		std::cerr<<"kernelSize cannot be even!"<<std::endl;
	}

	int paddingSize = (int) kernelSize / 2;
	Mat borderedImg;
	copyMakeBorder(img, borderedImg, paddingSize, paddingSize, paddingSize, paddingSize, BORDER_CONSTANT, UCHAR_MAX);
	Mat resultImg = borderedImg.clone();

	for(int i=paddingSize; i < borderedImg.rows - paddingSize; i++){
		for(int j=paddingSize; j < borderedImg.cols - paddingSize; j++){
			unsigned char currMin = UCHAR_MAX;
			for(int ii=-1*paddingSize; ii<=paddingSize; ii++){
				for(int jj=-1*paddingSize; jj<=paddingSize; jj++){
					if(borderedImg.at<uchar>(i + ii, j + jj) < currMin)
						currMin = borderedImg.at<uchar>(i + ii, j + jj);
				}
			}
			resultImg.at<uchar>(i, j) = currMin;
		}
	}

	return resultImg(Rect(paddingSize, paddingSize, img.cols, img.rows));
}


Mat maxFilter(const cv::Mat& img, int kernelSize){
	if (kernelSize % 2 == 0){
		std::cerr<<"kernelSize cannot be even!"<<std::endl;
	}

	int paddingSize = (int) kernelSize / 2;
	Mat borderedImg;
	copyMakeBorder(img, borderedImg, paddingSize, paddingSize, paddingSize, paddingSize, BORDER_CONSTANT, UCHAR_MAX);
	Mat resultImg = borderedImg.clone();

	for(int i=paddingSize; i < borderedImg.rows - paddingSize; i++){
		for(int j=paddingSize; j < borderedImg.cols - paddingSize; j++){
			unsigned char currMax = UCHAR_MIN;
			for(int ii=-1*paddingSize; ii<=paddingSize; ii++){
				for(int jj=-1*paddingSize; jj<=paddingSize; jj++){
					if(borderedImg.at<uchar>(i + ii, j + jj) > currMax)
						currMax = borderedImg.at<uchar>(i + ii, j + jj);
				}
			}
			resultImg.at<uchar>(i, j) = currMax;
		}
	}

	return resultImg(Rect(paddingSize, paddingSize, img.cols, img.rows));
}


Mat medianFilter(const Mat& img, int kernelSize){
	if (kernelSize % 2 == 0){
		std::cerr<<"kernelSize cannot be even!"<<std::endl;
	}

	int paddingSize = (int) kernelSize / 2;
	Mat borderedImg;
	copyMakeBorder(img, borderedImg, paddingSize, paddingSize, paddingSize, paddingSize, BORDER_CONSTANT, (int) ((UCHAR_MAX-UCHAR_MIN)/2));
	Mat resultImg = borderedImg.clone();

	for(int i=paddingSize; i < borderedImg.rows - paddingSize; i++){
		for(int j=paddingSize; j < borderedImg.cols - paddingSize; j++){
			std::vector<uchar> kernelValues;    // using a vector slows down the computation
			for(int ii=-1*paddingSize; ii<=paddingSize; ii++){
				for(int jj=-1*paddingSize; jj<=paddingSize; jj++){
					kernelValues.push_back(borderedImg.at<uchar>(i + ii, j + jj));
				}
			}
			std::sort(kernelValues.begin(), kernelValues.end());
			resultImg.at<uchar>(i, j) = kernelValues[paddingSize+1];
		}
	}

	return resultImg(Rect(paddingSize, paddingSize, img.cols, img.rows));
}