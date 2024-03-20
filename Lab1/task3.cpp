#include <opencv2/highgui.hpp>
#include <iostream>

using namespace cv;

void removeChannelAndShow(Mat img, int channel){
	if(img.channels() == 3){
		if(channel < 0 || channel > img.channels()-1){
			destroyAllWindows();
			throw std::range_error("Bad channel inputed");
		}
		for(int i=0; i<img.rows; i++){
			for(int j=0; j<img.cols; j++){
				img.at<Vec3b> (i,j)[channel] = 0;   // OpenCV uses BGR color space
			}
		}

		namedWindow("Removed channel");
		imshow("Removed channel", img);
		waitKey(0);


	}
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

	removeChannelAndShow(img, 0);

	return 0;
}