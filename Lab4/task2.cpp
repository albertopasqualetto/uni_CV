#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <iostream>

using namespace cv;


int main(int argc, char** argv){
	std::string filename = "street_scene.png";
	Mat src = imread(filename);
	if (src.data == NULL){
		std::cerr<<"The image cannot be read (because of missing file, improper permissions, unsupported or invalid format)"<<std::endl;
		return 1;
	}

	imshow("Original image", src);

	Mat greyImg;
	cvtColor(src, greyImg, COLOR_BGR2GRAY);

//	GaussianBlur(src, src, Size(3, 3), 0, 0);
//	imshow("Blurred image", src);

	Mat mask;
	inRange(greyImg, Vec<uchar, 1>(240), Vec<uchar, 1>(255), mask); // Pick the white color in the image
	imshow("Mask", mask);

	// Apply Sobel filter both in x and y directions to get diagonal edges
	Mat sobelImg;
	Sobel(mask, sobelImg, CV_64F, 1, 1, 7);
//	imshow("Sobel image", sobelImg);
	convertScaleAbs(sobelImg, sobelImg);    // Convert the result back to CV_8U

	// Create and apply a triangular mask to keep only the bottom-center of the image
	Mat centeringMask = Mat::zeros(mask.size(), CV_8UC1);
	fillConvexPoly(centeringMask, std::vector<Point>{Point(mask.cols/2, mask.rows/2), Point(1.0/4.0 * mask.cols, mask.rows), Point(3.0/4.0 * mask.cols, mask.rows)}, Scalar(255));
//	imshow("Centering mask", centeringMask);
	bitwise_and(sobelImg, centeringMask, sobelImg);

	// Erode and dilate the image to remove noise in lines and make them thicker
	Mat linesImg;
	Mat kernelErode = getStructuringElement(MORPH_CROSS, Size(5, 5));
	erode(sobelImg, linesImg, kernelErode);
	Mat kernelDilate = getStructuringElement(MORPH_ELLIPSE, Size(7, 7));
	dilate(linesImg, linesImg, kernelDilate);
	imshow("Lines image", linesImg);

	// Draw the lines on the original image
	src.setTo(Vec3b(0, 0, 255), linesImg);
	imshow("Result", src);

	waitKey(0);
	return 0;
}