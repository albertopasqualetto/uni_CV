#ifndef LAB2_IMAGEFILTERS_H
#define LAB2_IMAGEFILTERS_H

#include <opencv2/core/mat.hpp>

cv::Mat minFilter(const cv::Mat& img, int kernelSize);
cv::Mat maxFilter(const cv::Mat& img, int kernelSize);

cv::Mat medianFilter(const cv::Mat& img, int kernelSize);

#endif //LAB2_IMAGEFILTERS_H
