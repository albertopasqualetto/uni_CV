#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <iostream>

using namespace cv;

// Function to convert from the parameter space to the normal space
Vec<Point, 2> parameterSpaceToNormalSpace(Vec2f rhoTheta){
	float rho = rhoTheta[0], theta = rhoTheta[1];
	Point pt1, pt2;
	double a = cos(theta), b = sin(theta);
	double x0 = a*rho, y0 = b*rho;
	pt1.x = cvRound(x0 + 1300*(-b));
	pt1.y = cvRound(y0 + 1300*(a));
	pt2.x = cvRound(x0 - 1300*(-b));
	pt2.y = cvRound(y0 - 1300*(a));
	return Vec<Point, 2>{pt1, pt2};
}

// Function to find the intersection of two lines and return it through the intersection parameter
bool lineIntersection(const cv::Point2f &a1, const cv::Point2f &b1, const cv::Point2f &a2, const cv::Point2f &b2, cv::Point2f &intersection) {
	auto almostEqual = [](double number1, double number2) { // Function to compare floating point numbers
		double max = 1.0;
		if (std::abs(number1) > max)
			max = std::abs(number1);
		if (std::abs(number1) > max)
			max = std::abs(number2);
		return (std::abs(number1 - number2) <= (1E-5 * max));
	};


	double A1 = b1.y - a1.y;
	double B1 = a1.x - b1.x;
	double C1 = (a1.x * A1) + (a1.y * B1);

	double A2 = b2.y - a2.y;
	double B2 = a2.x - b2.x;
	double C2 = (a2.x * A2) + (a2.y * B2);

	double det = (A1 * B2) - (A2 * B1);


	if (!almostEqual(det, 0)) {
		intersection.x = static_cast<float>(((C1 * B2) - (C2 * B1)) / (det));
		intersection.y = static_cast<float>(((C2 * A1) - (C1 * A2)) / (det));

		return true;
	}

	return false;
}

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

	Mat edgesImg;
	Canny(greyscaleImg, edgesImg, 100, 255);    // Detect edges with Canny, thresholds set empirically

	// Standard Hough Line Transform
	std::vector<Vec2f> lines; // will hold the results of the detection
	HoughLines(edgesImg, lines, 1, CV_PI / 180, 150, 0, 0 ); // runs the actual detection

	// Copy edges to the images that will display the results in BGR
	Mat houghImg;
	cvtColor(edgesImg, houghImg, COLOR_GRAY2BGR);

	std::vector<Vec2f> delimitingLines;
	// Draw the lines
	bool lessThanHalfPILineAlreadyTaken = false;
	bool moreThanHalfPILineAlreadyTaken = false;
	for(size_t i = 0; i < lines.size(); i++){
		float rho = lines[i][0], theta = lines[i][1];
//		std::cout <<"Line "<< i<<" - rho: "<<rho<<", theta: "<<theta<<std::endl;
		bool lessThanHalfPI = (theta > 1.0/4.0 * CV_PI - 0.1 && theta < 1.0/4.0 * CV_PI + 0.1);
		bool moreThanHalfPI = (theta > 3.0/4.0 * CV_PI - 0.1 && theta < 3.0/4.0 * CV_PI + 0.1);
		if ((lessThanHalfPI && !lessThanHalfPILineAlreadyTaken) || (moreThanHalfPI && !moreThanHalfPILineAlreadyTaken)){
			if (lessThanHalfPI)
				lessThanHalfPILineAlreadyTaken = true;
			if (moreThanHalfPI)
				moreThanHalfPILineAlreadyTaken = true;
			delimitingLines.push_back(lines[i]);
			Vec<Point, 2> linePoints = parameterSpaceToNormalSpace(Vec2f{rho, theta});
			line(houghImg, linePoints[0], linePoints[1], Scalar(0, 0, 255), 1, LINE_8); // Draw the line (ensuring it is exactly red without antialiasing)
		}
	}
	imshow("Hough Transform", houghImg);
//	imwrite("hough_transform.png", houghImg);

	// Find the intersection of the delimiting lines
	Point2f intersection;
	bool intersecting = false;
	std::vector<Point> polyVertices;
	for (int i = 0; i < delimitingLines.size(); i++){
		for (int j = 1; j < delimitingLines.size(); j++){
			Vec<Point, 2> linePointsI = parameterSpaceToNormalSpace(delimitingLines[i]);
			Vec<Point, 2> linePointsJ = parameterSpaceToNormalSpace(delimitingLines[j]);
			intersecting = lineIntersection(linePointsI[0], linePointsI[1], linePointsJ[0], linePointsJ[1], intersection);
			if (intersecting){
				polyVertices.push_back(intersection);
//				std::cout<<"Vertex/intersection at: "<<intersection.x<<", "<<intersection.y<<std::endl;
			}
		}
	}

	Mat roadImg;
	srcImg.copyTo(roadImg);

	// Add the bottom vertices
	for (int j = 0; j < houghImg.cols; j++){
		if (houghImg.at<Vec3b>(houghImg.rows-1, j) == Vec3b(0, 0, 255)){
				polyVertices.push_back(Point(j, houghImg.rows - 1));
//				std::cout<<"Vertex at: "<<houghImg.rows-1<<", "<<j<<std::endl;
		}
	}
	// Draw the polygon
	fillPoly(roadImg, polyVertices, Scalar(0, 0, 255));

//	for (Point intersect : polyVertices)
//		circle(roadImg, intersect, 5, Scalar(0, 255, 0), 2);

	imshow("Road", roadImg);
//	imwrite("roadResult.png", roadImg);

	waitKey(0);
	return 0;
}
