#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <iostream>
#include "ImageFilters.h"

using namespace cv;

int main(int argc, char** argv){
	std::string filename = "Images/Garden.jpg";
	Mat img = imread(filename);
	if (img.data == NULL){
		std::cerr<<"The image cannot be read (because of missing file, improper permissions, unsupported or invalid format)"<<std::endl;
		return 1;
	}

	imshow("Original image", img);

	Mat grayImg = Mat();
	cvtColor(img, grayImg, COLOR_RGB2GRAY, 0);
	imshow("Grayscale image", grayImg);

	waitKey(0);

//	imwrite(filename.substr(0, filename.find('.')) + "_grayscale"+".png", grayImg);

	if (grayImg.channels() != 1){
		std::cerr<<"The image cannot be filtered because the number of channels is not 1"<<std::endl;
		return 1;
	}
	int minK = 3;
	Mat minFilteredImg = minFilter(grayImg, minK);
	imshow("Min-filtered image", minFilteredImg);
	int maxK = 3;
	Mat maxFilteredImg = maxFilter(grayImg, maxK);
	imshow("Max-filtered image", maxFilteredImg);
	// 2nd pass
	int minmaxK = 3;
	Mat minmaxFilteredImg = maxFilter(minFilteredImg, minmaxK);
	imshow("Min+Max-filtered image", minmaxFilteredImg);
	int maxminK = 3;
	Mat maxminFilteredImg = minFilter(maxFilteredImg, maxminK);
	imshow("Max+Min-filtered image", maxminFilteredImg);

	int gaussianK = 3;
 	Mat gaussianFilteredImg;
	GaussianBlur(grayImg, gaussianFilteredImg, Size(gaussianK, gaussianK), 0);
	imshow("Gaussian-filtered image", gaussianFilteredImg);
	imwrite(filename.substr(0, filename.find('.')) + "_gaussian"+std::to_string(gaussianK)+"filter"+".png", gaussianFilteredImg);
	waitKey(0);



	return 0;
}