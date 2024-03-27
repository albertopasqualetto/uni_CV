#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <iostream>

using namespace cv;

static void mouseCallbackPrintBGR(int event, int x, int y, int flags, void* img){
	if (event == EVENT_LBUTTONDOWN){
		Vec3b colorBGR = static_cast<Mat*>(img)->at<Vec3b>(y, x);   // row, col
		std::cout<<(int)colorBGR[0]<<','<<(int)colorBGR[1]<<','<<(int)colorBGR[2]<<std::endl;
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
	setMouseCallback("Original image", mouseCallbackPrintBGR, &img);

	waitKey(0);

	return 0;
}