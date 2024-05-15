#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <iostream>
#include <filesystem>
#include <opencv2/calib3d.hpp>
#include <opencv2/imgproc.hpp>

using namespace cv;
using namespace std;

namespace fs = std::filesystem;

bool hasEnding (std::string const &fullString, std::string const &ending) {
	if (fullString.length() >= ending.length()) {
		return (0 == fullString.compare (fullString.length() - ending.length(), ending.length(), ending));
	} else {
		return false;
	}
}


Mat mergeImages(const Mat &img1, const Mat &img2, bool verticalArrangement = false){
	int rows;
	int cols;

	if(verticalArrangement){
		rows = img1.rows + img2.rows;
		cols = max(img1.cols, img2.cols);
	} else{
		rows = max(img1.rows, img2.rows);
		cols = img1.cols + img2.cols;
	}

	cout<<"Rows: "<<rows<<" Cols: "<<cols<<endl;

	Mat outputMat(rows, cols, img1.type(), Vec3b(0, 0, 0));

	img1.copyTo(outputMat(Rect(0, 0, img1.cols, img1.rows)));
	img2.copyTo(outputMat(Rect(img1.cols, 0, img2.cols, img2.rows)));

	return outputMat;
}

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
	for (const auto &entry : fs::directory_iterator(argv[1])){
		if (hasEnding(entry.path().string(), ".jpg") ||
				hasEnding(entry.path().string(), ".jpeg") ||
				hasEnding(entry.path().string(), ".png") ||
				hasEnding(entry.path().string(), ".bmp") ||
				hasEnding(entry.path().string(), ".tiff") ||
				hasEnding(entry.path().string(), ".tif")) {
			filenames.push_back(entry.path().string());
		}
	}

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

		if(patternFound){
			Mat gray;
			cvtColor(img, gray, COLOR_BGR2GRAY);
			cornerSubPix(gray, corners, Size(11, 11), Size(-1, -1),
				TermCriteria(TermCriteria::Type::COUNT + TermCriteria::Type::EPS, 30, 0.1));
		}

		if(patternFound){
			imagePoints.push_back(corners);
		}

		drawChessboardCorners(img, patternSize, Mat(corners), patternFound);
		imshow("Image "+to_string(i), img);
	}
	waitKey(0);

	vector<vector<Point3f>> objectPoints(1);
	for (int i = 0; i < patternSize.height; ++i) {
		for (int j = 0; j < patternSize.width; ++j) {
			objectPoints[0].push_back(Point3f(j * squareSize, i * squareSize, 0));
		}
	}
	objectPoints[0][patternSize.width - 1].x = objectPoints[0][0].x + gridWidth;
	vector<Point3f> newObjPoints;
	newObjPoints = objectPoints[0];

	objectPoints.resize(imagePoints.size(),objectPoints[0]);


	Mat cameraMatrix, distCoeffs;
	vector<Mat> rvecs, tvecs;
	calibrateCamera(objectPoints, imagePoints, images[0].size(), cameraMatrix, distCoeffs, rvecs, tvecs);

	vector<float> perViewErrors;
	vector<Point2f> imagePoints2;
	int totalPoints = 0;
	double totalErr = 0, err;

	perViewErrors.resize(objectPoints.size());

	for(int i = 0; i < objectPoints.size(); ++i ){
		projectPoints(objectPoints[i], rvecs[i], tvecs[i], cameraMatrix, distCoeffs, imagePoints2);
		err = norm(imagePoints[i], imagePoints2, NORM_L2);
		perViewErrors[i] = static_cast<float>(std::sqrt(err*err/objectPoints[i].size()));
		totalErr += err;
		totalPoints += objectPoints[i].size();

	}

	double avg_reprojection_error = sqrt(totalErr/totalPoints);

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