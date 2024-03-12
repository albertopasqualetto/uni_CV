#include <opencv2/highgui.hpp>

int main(int argc, char** argv){
	const int dimGrad = 256;

	cv::Mat vertGrad(dimGrad, dimGrad, CV_8UC1);
	for(int i=0; i < dimGrad; i++){
		for (int j = 0; j < dimGrad; j++) {
			vertGrad.at<unsigned char>(i, j) = i;
		}
	}
	cv::namedWindow("Vertical Gradient");
	cv::imshow("Vertical Gradient", vertGrad);

	cv::Mat horGrad(dimGrad, dimGrad, CV_8UC1);
	for(int i=0; i < dimGrad; i++){
		for (int j = 0; j < dimGrad; j++) {
			horGrad.at<unsigned char>(i, j) = j;
		}
	}
	cv::namedWindow("Horizontal Gradient");
	cv::imshow("Horizontal Gradient", horGrad);

	cv::waitKey(0);


	const int dimChessboard = 300;

	const int dimSquare1 = 20;
	cv::Mat chessboard1(dimChessboard, dimChessboard, CV_8UC1);
	for (int i = 0; i < dimChessboard; i++) {
		for (int j=0; j<dimChessboard; j++){
			if (((int) floor(i/dimSquare1))%2  != ((int) floor(j/dimSquare1))%2){   // != is XOR
				chessboard1.at<unsigned char> (i,j) = 255;
			} else {
				chessboard1.at<unsigned char> (i,j) = 0;
			}
		}
	}
	cv::namedWindow("Chessboard1");
	cv::imshow("Chessboard1", chessboard1);

	const int dimSquare2 = 50;
	cv::Mat chessboard2(dimChessboard, dimChessboard, CV_8UC1);
	for (int i = 0; i < dimChessboard; i++) {
		for (int j=0; j<dimChessboard; j++){
			if (((int) floor(i/dimSquare2))%2  != ((int) floor(j/dimSquare2))%2){
				chessboard2.at<unsigned char> (i,j) = 255;
			} else {
				chessboard2.at<unsigned char> (i,j) = 0;
			}
		}
	}
	cv::namedWindow("Chessboard2");
	cv::imshow("Chessboard2", chessboard2);

	cv::waitKey(0);

	return 0;
}
