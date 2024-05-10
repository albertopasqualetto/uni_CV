#ifndef MATCHERS_H
#define MATCHERS_H

#include <opencv2/features2d.hpp>


void applyLoweRatio(cv::Ptr<cv::BFMatcher> bfMatcher, cv::Mat &descriptors1, cv::Mat &descriptors2, std::vector<cv::DMatch> &goodMatches, float ratio);


void printSimilarity(const std::vector<cv::DMatch> &matches, float similarityThreshold, float strongTransformThreshold);


cv::Mat bruteForceSIFTMatching(const cv::Mat &img1, const cv::Mat &img2, bool toPrintSimilarity);


cv::Mat bruteForceORBMatching(const cv::Mat &img1, const cv::Mat &img2, bool toPrintSimilarity);

#endif //MATCHERS_H
