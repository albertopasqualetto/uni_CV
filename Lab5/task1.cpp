#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main(int argc, char** argv){
	string filename = "Asphalt cracks/Asphalt-1.png";
	Mat src = imread(filename);
	if (src.data == NULL){
		cerr<<"The image cannot be read (because of missing file, improper permissions, unsupported or invalid format)"<<endl;
		return 1;
	}

	imshow("Original image", src);

	Mat grayscaleImg;
	cvtColor(src, grayscaleImg, COLOR_BGR2GRAY);
//	GaussianBlur(grayscaleImg, grayscaleImg, Size(25, 25), 0);
//	medianBlur(grayscaleImg, grayscaleImg, 19);

//	imshow("Grayscale image", grayscaleImg);
	Mat markerMask;
	cvtColor(src, markerMask, COLOR_BGR2GRAY);
	markerMask = Scalar::all(0);

//	Mat thresholdImg;
//	threshold(grayscaleImg, thresholdImg, 80, 255, THRESH_BINARY_INV);

//	Mat kernel = getStructuringElement(MORPH_CROSS, Size(5, 5));
//	erode(thresholdImg, thresholdImg, kernel);

//	src.setTo(Vec3b(0, 0, 255), thresholdImg);
//	imshow("Colored source", src);

//---------------------------------------------------


//	Mat kernel = (Mat_<float> (3, 3) <<
//			1,  1, 1,
//			1, -8, 1,
//			1,  1, 1);
//
//	Mat imgLaplacian;
//	Mat sharp = src;
//	filter2D(sharp, imgLaplacian, CV_32F, kernel);
//	src.convertTo(sharp, CV_32F);
//	Mat imgResult = sharp - imgLaplacian;
//
//	imgResult.convertTo(imgResult, CV_8UC3);
//	imgLaplacian.convertTo(imgLaplacian, CV_8UC3);
//
//	imshow("Sharped image", imgResult);
//
//
//	Mat imgBW;
//	cvtColor(src, imgBW, COLOR_BGR2GRAY);
//	threshold(imgBW, imgBW, 40, 255, THRESH_BINARY_INV | THRESH_OTSU);
//	Mat kernel = getStructuringElement(MORPH_CROSS, Size(3, 3));
//	erode(imgBW, imgBW, kernel);
//	imshow("Binary image", imgBW);
//
//
//	Mat dist;
//	distanceTransform(imgBW, dist, DIST_L2, 3);

//	normalize(dist, dist, 0, 1, NORM_MINMAX);
//	imshow("Distance transform image", dist);
//
//
//	threshold(dist, dist, 0.4, 1.0, THRESH_BINARY);
//
//	Mat kernel1 = Mat::ones(3, 3, CV_8UC1);
//	dilate(dist, dist, kernel1);
//	imshow("Peaks image", dist);
//
//
//	Mat dist_8u;
//	dist.convertTo(dist_8u, CV_8U);
//
//	vector<vector<Point>> contours;
//	findContours(dist_8u, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
//
//	Mat markers = Mat::zeros(dist.size(), CV_32SC1);
//
//	for(int i=0; i<contours.size(); i++){
//		drawContours(markers, contours, i, Scalar::all(i+1), -1);
//	}
//
//	circle(markers, Point(5, 5), 3, CV_RGB(255, 255, 255), -1, 8);
////	imshow("Markers", markers*10000);
//
//	watershed(src, markers);
//
//	Mat mark = Mat::zeros(markers.size(), CV_8UC1);
//	markers.convertTo(mark, CV_8UC1);
//	bitwise_not(mark, mark);
//	imshow("Markers again", mark);
//
//
//	vector<Vec3b> colors;
//	for(size_t i=0; i<contours.size(); i++){
//		int b = theRNG().uniform(0,255);
//		int g = theRNG().uniform(0,255);
//		int r = theRNG().uniform(0,255);
//
//		colors.push_back(Vec3b ((uchar)b, (uchar)g, (uchar)r));
//	}
//
//
//	Mat dst = Mat::zeros(markers.size(), CV_8UC3);
//
//	for(int i=0; i<markers.rows; i++){
//		for(int j=0; j<markers.cols; j++){
//			int idx = markers.at<int>(i, j);
//			if(idx>0 && idx<static_cast<int>(contours.size())){
//				dst.at<Vec3b>(i,j) = colors[idx-1];
//			} else{
//
//				dst.at<Vec3b>(i,j) = Vec3b(0,0,0);
//			}
//		}
//	}
//
//	imshow("Result image", dst);

	Mat imgHSV;
	Mat crack;
	cvtColor(src, imgHSV, COLOR_BGR2HSV);
	inRange(imgHSV, Vec3b(0, 0, 0), Vec3b(255, 255, 40), crack);
	imshow("From HSV", crack);

	Mat kernel = getStructuringElement(MORPH_CROSS, Size(3, 3));
//	erode(crack, crack, kernel);
	Mat opening;
	morphologyEx(crack, opening, MORPH_OPEN, kernel, Point(-1, -1), 2);
	imshow("Eroded img", opening);

	Mat sure_bg;
	dilate(opening, sure_bg, kernel, Point(-1, -1), 3);

	Mat dist_transform;
	distanceTransform(opening, dist_transform, DIST_L2, 5);
	double minDistTransform, maxDistTransform;
	cv::minMaxLoc(dist_transform, &minDistTransform, &maxDistTransform);
	Mat sure_fg;
	threshold(dist_transform, sure_fg, 0.7*maxDistTransform, 255, 0);

	Mat unknown = sure_bg - sure_fg;

	Mat markers;
	connectedComponents(sure_fg, markers);

	markers = markers+1;
	for(int i=0; i<markers.rows; i++){
		for(int j=0; j<markers.cols; j++){
			if(unknown.at<uchar>(i,j)==255){
				markers.at<uchar>(i,j)=0;
			}
		}
	}

	watershed(src, markers);

	//img[markers == -1] = [255,0,0]

	waitKey(0);
	return 0;
}