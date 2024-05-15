#include <filesystem>
#include <iostream>
#include <opencv2/core/mat.hpp>
#include <opencv2/imgcodecs.hpp>
#include "utils.h"


bool hasEnding (std::string const &fullString, std::string const &ending){
	if (fullString.length() >= ending.length()) {
		return (0 == fullString.compare (fullString.length() - ending.length(), ending.length(), ending));
	} else {
		return false;
	}
}


void getImagesFilenamesInFolder(const std::string &folderPath, std::vector<std::string> filenames){
	for (const auto &entry : std::filesystem::directory_iterator(folderPath)){
		if (hasEnding(entry.path().string(), ".jpg") ||
				hasEnding(entry.path().string(), ".jpeg") ||
				hasEnding(entry.path().string(), ".png") ||
				hasEnding(entry.path().string(), ".bmp") ||
				hasEnding(entry.path().string(), ".tiff") ||
				hasEnding(entry.path().string(), ".tif")) {
			filenames.push_back(entry.path().string());
		}
	}
}


void loadImages(const std::vector<std::string> &filenames, std::vector<cv::Mat> &images){
	for(int i=0; i<filenames.size(); i++){
		cv::Mat img = cv::imread(filenames[i]);
		if (!img.data){
			std::cerr<<"Image cannot be read (because of missing file, improper permissions, unsupported or invalid format)"<<std::endl;
			return;
		}
		images.push_back(img);
	}
}


cv::Mat mergeImages(const cv::Mat &img1, const cv::Mat &img2, bool verticalArrangement /*= false*/){
	unsigned int rows;
	unsigned int cols;

	if(verticalArrangement){
		rows = img1.rows + img2.rows;
		cols = std::max(img1.cols, img2.cols);
	} else{
		rows = std::max(img1.rows, img2.rows);
		cols = img1.cols + img2.cols;
	}

//	std::cout << "Rows: " << rows << " Cols: " << cols << std::endl;

	cv::Mat outputMat(rows, cols, img1.type(), cv::Vec3b(0, 0, 0));

	img1.copyTo(outputMat(cv::Rect(0, 0, img1.cols, img1.rows)));
	img2.copyTo(outputMat(cv::Rect(img1.cols, 0, img2.cols, img2.rows)));

	return outputMat;
}
