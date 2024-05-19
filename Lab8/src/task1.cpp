#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/imgproc.hpp>
#include "cameraCalibration.h"
#include "utils.h"

using namespace cv;
using namespace std;


/*
 * "images" folder:
 * patternSize: 9x6
 * squareSize: 22 mm
 * gridWidth: 177 mm
 * -> Camera matrix:
 * -> Average reprojection error:
 */

/*
 * "data" folder:
 * patternSize: 6x5
 * squareSize: 11 cm
 * -> Camera matrix:
 * -> Average reprojection error:
 */


const Size patternSize = Size(6, 5);
const float squareSize = 110;
const float gridWidth = -1;


int main(int argc, char** argv){
	if(argc != 2){
		cerr<<"A folder name shall be provided!"<<endl;
		return 1;
	}

	vector<string> filenames;
	getImagesFilenamesInFolder(argv[1], filenames);

	if(filenames.empty()){
		cerr<<"No images found in the folder!"<<endl;
		return 1;
	}

	vector<Mat> images;
	vector<vector<Point2f>> imagePoints;
	vector<Point3f> oneObjectPoint = createOneObjectPoints(patternSize, squareSize, gridWidth);
	vector<vector<Point3f>> objectPoints;

	loadImages(filenames, images, [&](int i){
		// Find chessboard corners
		vector<Point2f> corners;
		bool patternFound = findChessboardCorners(images[i], patternSize, corners, CALIB_CB_ADAPTIVE_THRESH + CALIB_CB_NORMALIZE_IMAGE);

		if(patternFound){
			Mat gray;
			cvtColor(images[0], gray, COLOR_BGR2GRAY);
			cornerSubPix(gray, corners, Size(11, 11), Size(-1, -1),
				TermCriteria(TermCriteria::Type::COUNT + TermCriteria::Type::EPS, 30, 0.1)
				);

			imagePoints.push_back(corners);
			objectPoints.push_back(oneObjectPoint);
		}


//		drawChessboardCorners(images[i], patternSize, Mat(corners), patternFound);
//		imshow("Image "+to_string(i), images[i]);
	});
//	waitKey(0);

	Mat cameraMatrix, distCoeffs;
	vector<Mat> rvecs, tvecs;
	calibrateCamera(objectPoints, imagePoints, images[0].size(), cameraMatrix, distCoeffs, rvecs, tvecs);

	Rect roi;
	Mat newCameraMatrix = getOptimalNewCameraMatrix(cameraMatrix, distCoeffs, images[0].size(), 0, images[0].size(), &roi);
	cout<<"Camera matrix: "<<cameraMatrix<<endl;
	cout<<"New Camera matrix: "<<newCameraMatrix<<endl;

	double avg_reprojection_error = calcAvgReprojErr(imagePoints, objectPoints, cameraMatrix, distCoeffs, rvecs, tvecs);
	cout<<"Average reprojection error: "<<avg_reprojection_error<<endl;

//	for(int i=0; i<images.size(); i++){
//		Mat undistortedImg;
//		undistort(images[i], undistortedImg, cameraMatrix, distCoeffs, newCameraMatrix);
//		cout<<"Image "<<i<<": "<<images[i].dims<<" "<<images[i].size<<" "<<images[i].type()<<endl;
//		cout << "Undistorted img: " << undistortedImg.dims << " " << undistortedImg.size << " " << undistortedImg.type() << endl;
//		Mat outputMat = mergeImages(images[i], undistortedImg);
//		imshow("out"+to_string(i), outputMat);
//		imwrite("outputs/out"+to_string(i)+".jpg", outputMat);
//	}
	Mat undistortedImg;
	Mat testImg = imread("data/test_image.png");
	undistort(testImg, undistortedImg, cameraMatrix, distCoeffs, newCameraMatrix);
	undistortedImg = undistortedImg(roi);
	Mat outputMat = mergeImages(testImg, undistortedImg);
	imshow("outTest", outputMat);
	imwrite("outputs/outTest.jpg", outputMat);


	waitKey(0);
	return 0;
}