#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <iostream>

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

	imwrite(filename.substr(0, filename.find('.')) + "_grayscale"+".png", grayImg);

	return 0;
}