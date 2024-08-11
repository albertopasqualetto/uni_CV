#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/calib3d.hpp>
#include <filesystem>
#include <iostream>

using namespace cv;
using namespace std;

int main()
{
  vector<Mat> images;
  const string folder = "data/checkerboard_images_piccole/";
  const string path = folder;
  const string type = ".png";
  filesystem::path p1 { path };
  int count = 0;
  Mat img;
  vector<vector<Point2f>> imgPoints;
  vector<Point2f> corners;
  for (auto& p : std::filesystem::directory_iterator(p1))
    count++;
  
  for(int i = 1; i < count; i++)
  {
    string pathImg = path + "" + format("%04d",i) + "_color"+ type;
    if(filesystem::exists(pathImg))
    {
      img = imread(pathImg);
      images.push_back(img);
    }
  }
  vector<vector<Point3f>> objPoints(images.size());
  for(int i = 0; i < images.size(); i++)
  {
    Size boardSize = Size(5,6);
    float squareSize = 0.11;
    for (int k = 0; k < boardSize.height; ++k) 
    {
      for (int j = 0; j < boardSize.width; ++j) 
      {
        objPoints[i].push_back(Point3f(j*squareSize, k*squareSize, 0));
      }
    }
    findChessboardCorners(images[i], boardSize, corners);
    imgPoints.push_back(corners);
  }
  for(int i = 0; i < imgPoints.size(); i++)
  {
    if(imgPoints[i].size() == 0)
    {
      objPoints.erase(objPoints.begin() + i);
      imgPoints.erase(imgPoints.begin() + i);
    }
  }
  Mat cameraMatrix, distCoeffs;
  vector<Mat> rvecs, tvecs;
  vector<double> stdDeviationsIntrinsics, stdDeviationsExtrinsics, perViewErrors;
  calibrateCamera(objPoints, imgPoints, images[0].size(), cameraMatrix, distCoeffs, rvecs, tvecs, stdDeviationsIntrinsics,
                    stdDeviationsExtrinsics, perViewErrors, 
                    CALIB_FIX_K3 | CALIB_FIX_K4 | CALIB_FIX_K5 | 
                    CALIB_FIX_K6 | CALIB_FIX_ASPECT_RATIO | 
                    CALIB_FIX_PRINCIPAL_POINT | CALIB_FIX_FOCAL_LENGTH | CALIB_FIX_INTRINSIC);

  printf("Camera Matrix: ");
  cout << cameraMatrix << endl;
  printf("Distortion Coefficients: ");
  cout << distCoeffs << endl;
  printf("Rvecs: ");
  cout << rvecs[0] << endl;
  printf("Tvecs: ");
  cout << tvecs[0] << endl;
  printf("Std Deviations Intrinsics: ");
  cout << stdDeviationsIntrinsics[0] << endl;
  printf("Std Deviations Extrinsics: ");
  cout << stdDeviationsExtrinsics[0] << endl;
  printf("Per View Errors: ");
  cout << perViewErrors[0] << endl;

  Mat mapUndistorted, rectified;
  vector<Mat> output(images.size());
  Mat dst;
  Mat newCameraMatrix = getOptimalNewCameraMatrix(cameraMatrix, distCoeffs, images[0].size(), 1, images[0].size());
  for(int i = 0; i < images.size(); i++)
  {
    Mat R = Mat::eye(3, 3, CV_32F);
    initUndistortRectifyMap(cameraMatrix, distCoeffs, R, newCameraMatrix , images[i].size(), CV_32FC1, mapUndistorted, rectified);
    remap(images[i], output[i], mapUndistorted, rectified, INTER_LINEAR);
    hconcat(images[i], output[i], dst);
    imwrite("../Output/Compare" + format("%02d",i)+".png", dst);
  }

  Mat imgTest = imread("data/test_image.png");
  Mat imgTestUndistorted;
  undistort(imgTest, imgTestUndistorted, cameraMatrix, distCoeffs, newCameraMatrix);
  hconcat(imgTest, imgTestUndistorted, dst);
  imwrite("test_image_undistorted.png", dst);
  
  return 0;
}