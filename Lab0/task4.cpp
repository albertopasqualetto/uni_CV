#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>
#include <iostream>

using namespace cv;

void showSingleChannels(Mat img){
	// img.type - 16 has only one channel https://stackoverflow.com/questions/10167534/how-to-find-out-what-type-of-a-mat-object-is-with-mattype-in-opencv
	Mat channels[3];
	split(img, channels);   // each Mat in channels has one channel

	namedWindow("Channel 1 (B)");
	imshow("Channel 1 (B)", channels[0]);
	namedWindow("Channel 2 (G)");
	imshow("Channel 2 (G)", channels[1]);
	namedWindow("Channel 3 (R)");
	imshow("Channel 3 (R)", channels[2]);
	waitKey(0);
}

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
	char key = waitKey(0);

	int n_channels = img.channels();

	std::cout<<"Number of channels: "<<n_channels<<std::endl;
	std::cout<<"Pressed key: "<<key<<std::endl;

	showSingleChannels(img);

	return 0;
}