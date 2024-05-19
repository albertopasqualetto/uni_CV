#ifndef CAMERACALIBRATION_H
#define CAMERACALIBRATION_H

std::vector<cv::Point3f> createOneObjectPoints(cv::Size patternSize, float squareSize, float gridWidth = -1);

double calcAvgReprojErr(const std::vector<std::vector<cv::Point2f>> &imagePoints, const std::vector<std::vector<cv::Point3f>> &objectPoints, const cv::Mat &cameraMatrix, const cv::Mat &distCoeffs, const std::vector<cv::Mat> &rvecs, const std::vector<cv::Mat> &tvecs);

#endif //CAMERACALIBRATION_H
