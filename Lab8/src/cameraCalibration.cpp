#include <vector>
#include <opencv2/core.hpp>
#include <opencv2/calib3d.hpp>
#include "cameraCalibration.h"

std::vector<std::vector<cv::Point3f>> createObjectPoints(cv::Size patternSize, float squareSize, float gridWidth, const std::vector<std::vector<cv::Point2f>> &imagePoints){
	std::vector<std::vector<cv::Point3f>> objectPoints(1);
	for (int i = 0; i < patternSize.height; ++i) {
		for (int j = 0; j < patternSize.width; ++j) {
			objectPoints[0].push_back(cv::Point3f(j * squareSize, i * squareSize, 0));
		}
	}
	objectPoints[0][patternSize.width - 1].x = objectPoints[0][0].x + gridWidth;
//	vector<Point3f> newObjPoints;
//	newObjPoints = objectPoints[0];

	objectPoints.resize(imagePoints.size(),objectPoints[0]);
	return objectPoints;
}


double calcAvgReprojErr(const std::vector<std::vector<cv::Point2f>> &imagePoints, const std::vector<std::vector<cv::Point3f>> &objectPoints, const cv::Mat &cameraMatrix, const cv::Mat &distCoeffs, const std::vector<cv::Mat> &rvecs, const std::vector<cv::Mat> &tvecs){
	std::vector<float> perViewErrors;
	std::vector<cv::Point2f> imagePoints2;
	unsigned int totalPoints = 0;
	double totalErr = 0;
	double err;

	perViewErrors.resize(objectPoints.size());

	for(int i = 0; i < objectPoints.size(); ++i ){
		cv::projectPoints(objectPoints[i], rvecs[i], tvecs[i], cameraMatrix, distCoeffs, imagePoints2);
		err = cv::norm(imagePoints[i], imagePoints2, cv::NORM_L2);
		perViewErrors[i] = static_cast<float>(sqrt(err * err / objectPoints[i].size()));
		totalErr += err;
		totalPoints += objectPoints[i].size();
	}

	return sqrt(totalErr/totalPoints);
}