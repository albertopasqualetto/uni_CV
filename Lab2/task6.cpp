#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <iostream>

using namespace cv;

int main(int argc, char** argv){
	std::string filename = "Images/Garden.jpg";
	Mat img = imread(filename);
	if (img.data == NULL){
		std::cerr<<"The image cannot be read (because of missing file, improper permissions, unsupported or invalid format)"<<std::endl;
		return 1;
	}

	imshow("Original image", img);

	Mat greyImg = Mat();
	cvtColor(img, greyImg, COLOR_RGB2GRAY, 0);
	imshow("Greyscale image", greyImg);
	waitKey(0);

//	imwrite(filename.substr(0, filename.find('.')) + "_grayscale"+".png", grayImg);

	Mat hist;
	int histSize = 255; // number of bins
	float range[] = {0, 255+1}; // range (upper bound is exclusive)
	const float* histRange[] = { range };
	calcHist(&greyImg, 1, 0, Mat(), hist, 1, &histSize, histRange);
	int hist_w = 512, hist_h = 400;
	int bin_w = cvRound( (double) hist_w/histSize );
	Mat histImg(hist_h, hist_w, CV_8UC1, Scalar(255));
	normalize(hist, hist, 0, histImg.rows, NORM_MINMAX, -1, Mat());
	for(int i = 1; i < histSize; i++){
		line(histImg, Point(bin_w * (i - 1), hist_h - cvRound(hist.at<float>(i - 1))),
		     Point(bin_w*(i), hist_h - cvRound(hist.at<float>(i))),
		     Scalar(0), 2, 8, 0);
	}
	imshow("Histogram", histImg);
//	imwrite(filename.substr(0, filename.find('.')) + "_histogramOneChannel"+std::to_string(histSize)+"bins"+".png", histImg);
	waitKey();

	Mat equalizedHistImg;
	equalizeHist(greyImg, equalizedHistImg);    // always 255 bins
	imshow("Equalized-histogram image", equalizedHistImg);
	imwrite(filename.substr(0, filename.find('.')) + "_eqHistOneChannel"+".png", equalizedHistImg);
	waitKey();

	return 0;
}