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
 * gridWidth: 199 mm
 */

/*
 * "data" folder:
 * patternSize: 6x5
 * squareSize: 11 cm
 * gridWidth: ~66 cm
 * -> Camera matrix: [1122.690781519136, 0, 699.4422259690488;
					 0, 1159.338656564772, 262.8058122486522;
					 0, 0, 1]
 * -> Average reprojection error: 1.50696
 */


const Size patternSize = Size(6, 5);
const float squareSize = 110;   // mm
const float gridWidth = 660;   // mm


int main(int argc, char** argv){
	if(argc != 2){
		cerr<<"A folder name and camera FoV shall be provided!"<<endl;
		return 1;
	}

	vector<string> filenames;
	getImagesFilenamesInFolder(argv[1], filenames);

	vector<vector<Point2f>> imagePoints;

	if(filenames.empty()){
		cerr<<"No images found in the folder!"<<endl;
		return 1;
	}
	vector<Mat> images;
	for(int i=0; i<filenames.size(); i++){
		Mat img = imread(filenames[i]);
		if (!img.data){
			cerr<<"Image cannot be read (because of missing file, improper permissions, unsupported or invalid format)"<<endl;
			return 1;
		}
		images.push_back(img);

		// Find chessboard corners
		vector<Point2f> corners;
		bool patternFound = findChessboardCorners(img, patternSize, corners, CALIB_CB_ADAPTIVE_THRESH + CALIB_CB_NORMALIZE_IMAGE);

//		if(patternFound){
//			Mat gray;
//			cvtColor(img, gray, COLOR_BGR2GRAY);
//			cornerSubPix(gray, corners, Size(11, 11), Size(-1, -1),
//				TermCriteria(TermCriteria::Type::COUNT + TermCriteria::Type::EPS, 30, 0.1));
//		}

		if(patternFound){
			imagePoints.push_back(corners);
		}

//		drawChessboardCorners(img, patternSize, Mat(corners), patternFound);
//		imshow("Image "+to_string(i), img);
	}
	waitKey(0);

	vector<vector<Point3f>> objectPoints = createObjectPoints(patternSize, squareSize, gridWidth, imagePoints);

	Mat cameraMatrix, distCoeffs;
	vector<Mat> rvecs, tvecs;
	calibrateCamera(objectPoints, imagePoints, images[0].size(), cameraMatrix, distCoeffs, rvecs, tvecs);

	double avg_reprojection_error = calcAvgReprojErr(imagePoints, objectPoints, cameraMatrix, distCoeffs, rvecs, tvecs);

	cout<<"Camera matrix: "<<cameraMatrix<<endl;
	cout<<"Average reprojection error: "<<avg_reprojection_error<<endl;


	Mat map1, map2;
	initUndistortRectifyMap(cameraMatrix, vector<int>(), Mat(), cameraMatrix, images[0].size(), CV_16SC2, map1, map2);

	for(int i=0; i<images.size(); i++){
		Mat remappedMat;
		remap(images[i], remappedMat, map1, map2, INTER_LINEAR);
		cout<<"Image "<<i<<": "<<images[i].dims<<" "<<images[i].size<<" "<<images[i].type()<<endl;
		cout<<"Remapped img: "<<remappedMat.dims<<" "<<remappedMat.size<<" "<<remappedMat.type()<<endl;
		Mat outputMat = mergeImages(images[i], remappedMat);
		imshow("out"+to_string(i), outputMat);
		imwrite("outputs/out"+to_string(i)+".jpg", outputMat);
	}
	Mat remappedMat;
	Mat testImg = imread("data/test_image.png");
	remap(testImg, remappedMat, map1, map2, INTER_LINEAR);
	Mat outputMat = mergeImages(testImg, remappedMat);
	imshow("outTest", outputMat);
	imwrite("outputs/outTest.jpg", outputMat);


//	imwrite("out.jpg", outputMat);

	waitKey(0);
	return 0;
}