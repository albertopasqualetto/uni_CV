#include "match.h"
#include <opencv2/calib3d.hpp>
#include <opencv2/highgui.hpp>


void applyLoweRatio(const cv::Ptr<cv::BFMatcher>& bfMatcher, cv::Mat &descriptors1, cv::Mat &descriptors2, std::vector<cv::DMatch> &goodMatches, float ratio=0.75){
	std::vector<std::vector<cv::DMatch>> matches;
	bfMatcher->knnMatch(descriptors1, descriptors2, matches, 2);
	for(std::vector<cv::DMatch> match : matches){
		if(match[0].distance < ratio*match[1].distance){ //match[0] and match[1] are the two best matches for a given keypoint. The condition match[0].distance < 0.75*match[1].distance is used to filter out bad matches. Only the best match (match[0]) is kept if its distance is significantly smaller than the second best match (match[1]).
//			cout<<"Matched keypoints: "<<match[0].queryIdx<<" and "<<match[0].trainIdx<<endl;
			goodMatches.push_back(match[0]);
		}
	}
}


void SIFTMatchingKeypoints(const cv::Mat &img1, const cv::Mat &img2, std::vector<cv::DMatch> &goodMatches, std::vector<cv::KeyPoint> &keypoints1, std::vector<cv::KeyPoint> &keypoints2) {
	cv::Ptr<cv::SIFT> sift = cv::SIFT::create();

//	std::vector<cv::KeyPoint> keypoints1, keypoints2;
	cv::Mat descriptors1, descriptors2;
	sift->detectAndCompute(img1, cv::noArray(), keypoints1, descriptors1);
	sift->detectAndCompute(img2, cv::noArray(), keypoints2, descriptors2);

	cv::Ptr<cv::BFMatcher> bfMatcher = cv::BFMatcher::create();

//	std::vector<cv::DMatch> goodMatches;
	applyLoweRatio(bfMatcher, descriptors1, descriptors2, goodMatches);
}


double getTranslationFromMatches(const std::vector<cv::DMatch> &goodMatches, const std::vector<cv::KeyPoint> &keypoints1, const std::vector<cv::KeyPoint> &keypoints2){
	std::vector<cv::Point2f> points1, points2;
	for(int i = 0; i < goodMatches.size(); i++){   // Get the keypoints from the good matches
		points1.push_back(keypoints1[goodMatches[i].queryIdx].pt);
		points2.push_back(keypoints2[goodMatches[i].trainIdx].pt);
	}

	cv::Mat maskInliersOutliers;
	cv::Mat H = findHomography(points1, points2, maskInliersOutliers, cv::RANSAC);
//	std::cout<<"Homography matrix: "<<H<<std::endl;

//	std::vector<cv::DMatch> inlierMatches;
//	float avgTranslation = 0;
//	int inliers_n = 0;
//	for(int i = 0; i < goodMatches.size(); i++){    // keep only inliers
//		if(maskInliersOutliers.at<bool>(i)==1){
////			inlierMatches.push_back(goodMatches[i]);
//			avgTranslation += goodMatches[i].distance;
//			inliers_n++;
//		}
//	}
//	avgTranslation /= inliers_n;
//	return avgTranslation;

	return H.at<double>(0,2);
}


cv::Mat createStitching(const std::vector<cv::Mat> &images, const std::vector<int> &translations, int tot_cols){
	cv::Mat outputMat = cv::Mat(images[0].rows, tot_cols, images[0].type());

	int cols_acc = 0;
	for(int i=0; i<images.size()-1; i++){
		cv::Mat subM = outputMat(cv::Range(0, outputMat.rows), cv::Range(cols_acc, cols_acc + images[i].cols));
//		Mat subM = outputMat(Range(0, outputMat.rows), Range(cols_acc, cols_acc + images[i].cols));
		images[i].copyTo(subM);

		cols_acc += translations[i];
	}
	cv::Mat subM =  outputMat(cv::Range(0, outputMat.rows), cv::Range(cols_acc, cols_acc + images[images.size()-1].cols));
	images[images.size()-1].copyTo(subM);

	return outputMat;
}
