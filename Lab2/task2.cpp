#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <iostream>

using namespace cv;

int main(int argc, char** argv){
	Mat img = imread("C:\\Users\\alber\\Desktop\\Computer Vision\\Lab\\Lab2\\Images\\Garden.jpg");
	if (img.data == NULL){
		std::cerr<<"The image cannot be read (because of missing file, improper permissions, unsupported or invalid format"<<std::endl;
		return 1;
	}

	imshow("Original image", img);

	Mat greyImg = Mat();
	cvtColor(img, greyImg, COLOR_RGB2GRAY, 0);
	imshow("Greyscale image", greyImg);

	waitKey(0);

	imwrite("C:\\Users\\alber\\Desktop\\Computer Vision\\Lab\\Lab2\\Images\\Garden_grayscale.jpg", greyImg);

	return 0;
}