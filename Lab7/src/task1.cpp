#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <iostream>
#include <filesystem>
#include "panoramic_utils.h"
#include "match.h"
#include <opencv2/calib3d.hpp>
#include <opencv2/imgproc.hpp>

using namespace cv;
using namespace std;

namespace fs = std::filesystem;

bool hasEnding (std::string const &fullString, std::string const &ending) {
	if (fullString.length() >= ending.length()) {
		return (0 == fullString.compare (fullString.length() - ending.length(), ending.length(), ending));
	} else {
		return false;
	}
}


int main(int argc, char** argv){
	if(argc != 3){
		cerr<<"A folder name and camera FoV shall be provided!"<<endl;
		return 1;
	}


	vector<string> filenames;
	for (const auto &entry : fs::directory_iterator(argv[1])){
		if (hasEnding(entry.path().string(), ".jpg") ||
				hasEnding(entry.path().string(), ".jpeg") ||
				hasEnding(entry.path().string(), ".png") ||
				hasEnding(entry.path().string(), ".bmp") ||
				hasEnding(entry.path().string(), ".tiff") ||
				hasEnding(entry.path().string(), ".tif")) {
			filenames.push_back(entry.path().string());
		}
	}

	float FOV = strtof(argv[2], nullptr);

	vector<Mat> images;
	for(int i=0; i<filenames.size(); i++){
		Mat img = imread(filenames[i]);
		if (!img.data){
			cerr<<"Image cannot be read (because of missing file, improper permissions, unsupported or invalid format)"<<endl;
			return 1;
		}
		img = cylindricalProj(img, FOV/2);
//		equalizeHist(img, img);
		images.push_back(img);
//		imshow("Image "+to_string(i), img);
	}


	int tot_cols = images[0].cols;
	vector<int> translations;
	for(int i=0; i<images.size()-1; i++){
		vector<DMatch> goodMatches;
		vector<KeyPoint> keypoints1, keypoints2;
		SIFTMatchingKeypoints(images[i], images[i + 1], goodMatches, keypoints1, keypoints2);
		double dist = getTranslationFromMatches(goodMatches, keypoints1, keypoints2);
		dist = abs(dist);
		translations.push_back(dist);
		tot_cols += dist;
//		cout<<"Image "<<i<<" dist: "<<dist<<endl;
//		imshow("Image "+to_string(i), matches);
	}

	Mat outputMat = createStitching(images, translations, tot_cols);

	imshow("Stitched Image", outputMat);
	imwrite("out_"+string(argv[1]).substr(string(argv[1]).find('/')+1)+"_stitched.jpg", outputMat);

	waitKey(0);
	return 0;
}