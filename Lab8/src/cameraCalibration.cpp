#include <vector>
#include <opencv2/core.hpp>
#include <opencv2/calib3d.hpp>
#include <iostream>
#include "cameraCalibration.h"

// gridWidth used to refine total width of the grid
std::vector<cv::Point3f> createOneObjectPoints(cv::Size patternSize, float squareSize, float gridWidth /*= -1*/){
	std::vector<cv::Point3f> objectPoints;
	for (int i = 0; i < patternSize.height; ++i) {
		for (int j = 0; j < patternSize.width; ++j) {
			objectPoints.push_back(cv::Point3f(j * squareSize, i * squareSize, 0));
		}
	}

	if (gridWidth != -1){
		objectPoints[patternSize.width - 1].x = objectPoints[0].x + gridWidth;
	}

	return objectPoints;
}


double calcAvgReprojErr(const std::vector<std::vector<cv::Point2f>> &imagePoints, const std::vector<std::vector<cv::Point3f>> &objectPoints, const cv::Mat &cameraMatrix, const cv::Mat &distCoeffs, const std::vector<cv::Mat> &rvecs, const std::vector<cv::Mat> &tvecs){
	std::vector<cv::Point2f> imagePoints2;
	double totalErr = 0;
	double err;

	for(int i = 0; i < objectPoints.size(); ++i ){
		cv::projectPoints(objectPoints[i], rvecs[i], tvecs[i], cameraMatrix, distCoeffs, imagePoints2);
		err = cv::norm(imagePoints[i], imagePoints2, cv::NORM_L2) / imagePoints2.size();
		totalErr += err;
	}

	return totalErr/objectPoints.size();
}