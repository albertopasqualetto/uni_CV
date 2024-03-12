#include <opencv2/highgui.hpp>
#include <iostream>

using namespace cv;

int main(int argc, char** argv){
	if(argc < 2){
		std::cerr<<"An image filename shall be provided!"<<std::endl;
		return 1;
	}

	Mat img = imread(argv[1]);
	if (img.data == NULL){
		std::cerr<<"The image cannot be read (because of missing file, improper permissions, unsupported or invalid format"<<std::endl;
		return 1;
	}
	namedWindow("Example 1");
	imshow("Example 1", img);
	waitKey(0);
	return 0;
}