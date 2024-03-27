#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <iostream>

using namespace cv;

static void mouseCallbackPrintBGR(int event, int x, int y, int flags, void* img){
	if (event == EVENT_LBUTTONDOWN){
		Mat image = *static_cast<Mat*>(img);

		Vec3b colorBGR = image.at<Vec3b>(y, x);   // row, col
		std::cout<<"This pixel BGR values: ("<<(int)colorBGR[0]<<", "<<(int)colorBGR[1]<<", "<<(int)colorBGR[2]<<")"<<std::endl;

		const int BORDER = 9;
		int radius = BORDER / 2;
		int sumB = 0;
		int sumG = 0;
		int sumR = 0;
		int pixelCount = 0;
		for(int j = -1 * radius; j <= radius; j++) {
			if (y + j < 0 || y + j >= image.rows)
				continue;
			for (int i = -1 * radius; i <= radius; i++) {
				if (x + i < 0 || x + i >= image.cols)
					continue;
				sumB += (int) image.at<Vec3b>(y+j, x+i)[0];
				sumG += (int) image.at<Vec3b>(y+j, x+i)[1];
				sumR += (int) image.at<Vec3b>(y+j, x+i)[2];
				pixelCount++;
			}
		}
		std::cout<<"This "<<BORDER<<'x'<<BORDER<<" neighborhood BGR values: ("<<sumB/pixelCount<<", "<<sumG/pixelCount<<", "<<sumR/pixelCount<<")"<<std::endl;
	}
}

int main(int argc, char** argv){
	std::string filename = "robocup.jpg";
	Mat img = imread(filename);
	if (img.data == NULL){
		std::cerr<<"The image cannot be read (because of missing file, improper permissions, unsupported or invalid format)"<<std::endl;
		return 1;
	}

	imshow("Original image", img);
	setMouseCallback("Original image", mouseCallbackMeanBGR, &img);

	waitKey(0);

	return 0;
}