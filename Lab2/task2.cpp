#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <iostream>
#include "ImageFilters.h"

using namespace cv;

int main(int argc, char** argv){
	std::string filename = "Images/Lena_corrupted.png";
	Mat img = imread(filename);
	if (img.data == NULL){
		std::cerr<<"The image cannot be read (because of missing file, improper permissions, unsupported or invalid format)"<<std::endl;
		return 1;
	}

	imshow("Original image", img);

	Mat grayImg = Mat();
	cvtColor(img, grayImg, COLOR_RGB2GRAY, 0);
	imshow("Grayscale image", grayImg);

	waitKey(0);

//	imwrite(filename.substr(0, filename.find('.')) + "_grayscale"+".png", grayImg);

	if (grayImg.channels() != 1){
		std::cerr<<"The image cannot be filtered because the number of channels is not 1"<<std::endl;
		return 1;
	}
	int minK = 3;
	Mat minFilteredImg = minFilter(grayImg, minK);
	imshow("Min-filtered image", minFilteredImg);
	imwrite(filename.substr(0, filename.find('.')) + "_min"+std::to_string(minK)+"filter"+".png", minFilteredImg);
	int maxK = 3;
	Mat maxFilteredImg = maxFilter(grayImg, maxK);
	imshow("Max-filtered image", maxFilteredImg);
	imwrite(filename.substr(0, filename.find('.')) + "_max"+std::to_string(maxK)+"filter"+".png", maxFilteredImg);
	// 2nd pass
	int minmaxK = 3;
	Mat minmaxFilteredImg = maxFilter(minFilteredImg, minmaxK);
	imshow("Min+Max-filtered image", minmaxFilteredImg);
	imwrite(filename.substr(0, filename.find('.')) + "_min"+std::to_string(minK)+"max"+std::to_string(minmaxK)+"filter"+".png", minmaxFilteredImg);
	int maxminK = 3;
	Mat maxminFilteredImg = minFilter(maxFilteredImg, maxminK);
	imshow("Max+Min-filtered image", maxminFilteredImg);
	imwrite(filename.substr(0, filename.find('.')) + "_max"+std::to_string(maxK)+"min"+std::to_string(maxminK)+"filter"+".png", maxminFilteredImg);
	waitKey(0);

	// Best removes noise from Lena_corrupted: applying both max and min filters one after the other with kernel 3, the result is bright with max->min and dark vice-versa; applying only one filter the results are very bad in both cases (bigger kernels make worse results)
	// Best removes noise from Astronaut_salt_pepper: applying a single filter makes the image appearance even worse (using kernel 3, the one with best results), applying both filters one after the other the salt (min->max) or the pepper (max->min) is almost removed and the other is not affected. If using larger kernels in the second pass (5 is the sweet spot), the image content is more clear since it has less white/black peaks, but it is more blurry and many small squares can be seen.
	// Best removes the electric cables in the background from Garden: applying a max filter with kernel 3 removes all but one cables without any artifact, with kernel 5 it removes all cables introducing some artifacts especially on the tree; if a min filter kernel 3 is applied after the max with kernel 5, the results are even better, reducing artifacts on the tree. If min filter is applied first, electric cables are not removed; increasing kernel size, it introduces too much artifacts.

	return 0;
}