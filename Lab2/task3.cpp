#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <iostream>
#include "ImageFilters.h"

using namespace cv;

int main(int argc, char** argv){
	std::string filename = "Images/Astronaut_salt_pepper.png";
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

	int medianK = 5;
	Mat medianFilteredImg = medianFilter(grayImg, medianK);
	imshow("Median-filtered image", medianFilteredImg);
	imwrite(filename.substr(0, filename.find('.')) + "_median"+std::to_string(medianK)+"filter"+".png", medianFilteredImg);
	waitKey(0);

	// Remove noise from Lena_corrupted: median filter with kernel 3 performs good with respect to min and max filters giving a better representation of the content, also kernel 5 is good enhancing the visualization trading the sharpness of borders
	// Remove noise from Astronaut_salt_pepper: median filter with kernel 3 is better than corrupted one, but it is similar to two pass min max filter with kernel 3
	// Remove electric cables from Garden: median filter does not help with any kernel


	return 0;
}