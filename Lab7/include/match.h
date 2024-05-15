#ifndef MATCH_H
#define MATCH_H

#include <opencv2/features2d.hpp>


void applyLoweRatio(const cv::Ptr<cv::BFMatcher>& bfMatcher, cv::Mat &descriptors1, cv::Mat &descriptors2, std::vector<cv::DMatch> &goodMatches, float ratio);

void SIFTMatchingKeypoints(const cv::Mat &img1, const cv::Mat &img2, std::vector<cv::DMatch> &goodMatches, std::vector<cv::KeyPoint> &keypoints1, std::vector<cv::KeyPoint> &keypoints2);

double getTranslationFromMatches(const std::vector<cv::DMatch> &goodMatches, const std::vector<cv::KeyPoint> &keypoints1, const std::vector<cv::KeyPoint> &keypoints2);

cv::Mat createStitching(const std::vector<cv::Mat> &images, const std::vector<int> &translations, int tot_cols);

#endif //MATCH_H
