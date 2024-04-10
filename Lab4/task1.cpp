#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <iostream>

using namespace cv;

const char* window_name = "Edge Map";
const int RATIO = 3;    // as recommended by OpenCV documentation

static void CannyThreshold(int lowThreshold, void* img){
	Mat dst;
	Canny(*static_cast<Mat*>(img), dst, lowThreshold, lowThreshold*RATIO);

	imshow(window_name, dst);
}


int main(int argc, char** argv){
	std::string filename = "street_scene.png";
	Mat src = imread(filename);
	if (src.data == NULL){
		std::cerr<<"The image cannot be read (because of missing file, improper permissions, unsupported or invalid format)"<<std::endl;
		return 1;
	}

	imshow("Original image", src);

//	cvtColor(src, src_gray, COLOR_BGR2GRAY);

	Mat dst;

	const int max_lowThreshold = 255;

	namedWindow(window_name, WINDOW_AUTOSIZE);
	createTrackbar("Min Threshold", window_name, NULL, max_lowThreshold, CannyThreshold, &src);
	setTrackbarPos("Min Threshold", window_name, 128);

	waitKey(0);
	return 0;
}