#ifndef UTILS_H
#define UTILS_H

bool hasEnding (std::string const &fullString, std::string const &ending);

cv::Mat mergeImages(const cv::Mat &img1, const cv::Mat &img2, bool verticalArrangement = false);

void getImagesFilenamesInFolder(const std::string &folderPath, std::vector<std::string> &filenames);

void loadImages(const std::vector<std::string> &filenames, std::vector<cv::Mat> &images, const std::function<void(int)>& func = nullptr);

#endif //UTILS_H
