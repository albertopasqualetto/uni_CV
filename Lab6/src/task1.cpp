#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <iostream>
#include "matchers.h"

using namespace cv;
using namespace std;


int main(int argc, char** argv){
	if(argc != 3){
		cerr<<"Two image filenames shall be provided!"<<endl;
		return 1;
	}

	Mat img1 = imread(argv[1]);
	Mat img2 = imread(argv[2]);

	if (!img1.data || !img2.data){
		cerr<<"Images cannot be read (because of missing file, improper permissions, unsupported or invalid format)"<<endl;
		return 1;
	}

	imshow("Image 1", img1);
	imshow("Image 2", img2);

	cout<<"SIFT"<<endl;
	Mat siftImg = bruteForceSIFTMatching(img1.clone(), img2.clone(), true);
	imshow("SIFT features image", siftImg);

	cout<<"ORB"<<endl;
	Mat orbImg = bruteForceORBMatching(img1.clone(), img2.clone(), true);
	imshow("ORB features image", orbImg);

	waitKey(0);
	return 0;
}