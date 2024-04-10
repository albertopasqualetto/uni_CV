#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <iostream>

using namespace cv;


// Gives the equation of the line passing through two points in the form y = mx + q
void equationFormula(double x1, double y1, double x2, double y2, double &m, double &q){
	m = (y2-y1)/(x2-x1);
	q = -x1*m + y1;
}


int main(int argc, char** argv){
	std::string filename = "street_scene.png";
	Mat src = imread(filename);
	if (src.data == NULL){
		std::cerr<<"The image cannot be read (because of missing file, improper permissions, unsupported or invalid format)"<<std::endl;
		return 1;
	}
//	drawMarker(src, Point(src.cols/2, src.rows*4/6), Scalar(0, 255, 0), MARKER_CROSS, 10, 1);
//	drawMarker(src, Point(src.cols/2, src.rows*5/6), Scalar(0, 255, 0), MARKER_CROSS, 10, 1);
//	line(src, Point(src.cols/2 -1000, src.rows*4/6), Point(src.cols/2 +1000, src.rows*4/6), Scalar(0, 255, 0), 1, LINE_8);
//	line(src, Point(src.cols/2 -1000, src.rows*5/6), Point(src.cols/2 +1000, src.rows*5/6), Scalar(0, 255, 0), 1, LINE_8);
	imshow("Original image", src);

	Mat mask;
//	Mat hsvImg;
//	cvtColor(src, hsvImg, COLOR_BGR2HSV);
//	inRange(hsvImg, Vec3b(0, 0, 230), Vec3b(255, 0, 255), mask);
	inRange(src, Vec3b(240, 240, 240), Vec3b(255, 255, 255), mask); // Pick the white color in the image

	// Find the 2 points of the middle (left) line and 2 of the right one in the mask by starting from the horizontal middle and going left and right at different heights, selected empirically at x=4/6 and x=5/6 of the image height
	Point l1, l2, r1, r2;
	for(int j=mask.cols/2; j>=0; j--){
		if(mask.at<uchar>(src.rows*4/6, j)==255){
			l1.x = j;
			l1.y = src.rows*4/6;
			break;
		}
	}
	for(int j=mask.cols/2; j>=0; j--){
		if(mask.at<uchar>(src.rows*5/6, j)==255){
			l2.x = j;
			l2.y = src.rows*5/6;
			break;
		}
	}
	for(int j=mask.cols/2; j<mask.cols; j++){
		if(mask.at<uchar>(src.rows*4/6, j)==255){
			r1.x = j;
			r1.y = src.rows*4/6;
			break;
		}
	}
	for(int j=mask.cols/2; j<mask.cols; j++){
		if(mask.at<uchar>(src.rows*5/6, j)==255){
			r2.x = j;
			r2.y = src.rows*5/6;
			break;
		}
	}

	Mat linesImg = src.clone();
	// get the equations of the lines
	double ml, ql, mr, qr;
	equationFormula(l1.x, l1.y, l2.x, l2.y, ml, ql);
	equationFormula(r1.x, r1.y, r2.x, r2.y, mr, qr);

	//draw the lines from the middle of the image to the bottom
	line(linesImg, Point(linesImg.cols/2, linesImg.cols/2 *ml +ql), Point(0, 0 *ml +ql), Scalar(0, 0, 255), 2, LINE_AA);
	line(linesImg, Point(linesImg.cols/2, linesImg.cols/2 *mr +qr), Point(linesImg.cols, linesImg.cols *mr +qr), Scalar(0, 0, 255), 2, LINE_AA);

	imshow("Lines image", linesImg);


	waitKey(0);
	return 0;
}