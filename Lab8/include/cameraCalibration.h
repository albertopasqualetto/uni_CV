#ifndef CAMERACALIBRATION_H
#define CAMERACALIBRATION_H

std::vector<std::vector<cv::Point3f>> createObjectPoints(cv::Size patternSize, float squareSize, float gridWidth, const std::vector<std::vector<cv::Point2f>> &imagePoints);

double calcAvgReprojErr(const std::vector<std::vector<cv::Point2f>> &imagePoints, const std::vector<std::vector<cv::Point3f>> &objectPoints, const cv::Mat &cameraMatrix, const cv::Mat &distCoeffs, const std::vector<cv::Mat> &rvecs, const std::vector<cv::Mat> &tvecs);

#endif //CAMERACALIBRATION_H
