#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <iostream>

using namespace cv;


int main(int argc, char** argv){
	std::string filename = "street_scene.png";
	Mat srcImg = imread(filename);
	if (srcImg.data == NULL){
		std::cerr<<"The image cannot be read (because of missing file, improper permissions, unsupported or invalid format)"<<std::endl;
		return 1;
	}

	imshow("Original image", srcImg);

	// Convert the image to greyscale for use in Canny edge detection
	Mat greyscaleImg;
	cvtColor(srcImg, greyscaleImg, COLOR_BGR2GRAY);

	GaussianBlur(greyscaleImg, greyscaleImg, Size(7, 7), 1.5); // Smooth the image to reduce false circles
//	medianBlur(greyscaleImg, greyscaleImg, 5); // Smooth the image to reduce false circles
	Mat opImg;
	cvtColor(greyscaleImg, opImg, COLOR_GRAY2BGR);

	// Hough Circular Transform
	Mat signImg;
	srcImg.copyTo(signImg);
	std::vector<Vec3f> circles; // will hold the results of the detection
	HoughCircles(greyscaleImg, circles, HOUGH_GRADIENT, 2, 10, 200, 30, 7, 12); // Empirically determined parameters

	for(size_t i = 0; i < circles.size(); i++){
		Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
		int radius = cvRound(circles[i][2]);
		std::cout<<"Circle "<<i+1<<": Center = ("<<center.x<<", "<<center.y<<"), Radius = "<<radius<<std::endl;
		// draw the circle center
		circle(signImg, center, 3, Scalar(0,255,0), -1, 8, 0);
		// draw the circle outline
		circle(signImg, center, radius, Scalar(0,0,255), 3, 8, 0);
	}

	imshow("Road sign", signImg);
//	imwrite("signResult.png", signImg);

	waitKey(0);
	return 0;
}
