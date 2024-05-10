#include "matchers.h"
#include <iostream>


void applyLoweRatio(cv::Ptr<cv::BFMatcher> bfMatcher, cv::Mat &descriptors1, cv::Mat &descriptors2, std::vector<cv::DMatch> &goodMatches, float ratio=0.75){
	std::vector<std::vector<cv::DMatch>> matches;
	bfMatcher->knnMatch(descriptors1, descriptors2, matches, 2);
	for(std::vector<cv::DMatch> match : matches){
		if(match[0].distance < ratio*match[1].distance){ //match[0] and match[1] are the two best matches for a given keypoint. The condition match[0].distance < 0.75*match[1].distance is used to filter out bad matches. Only the best match (match[0]) is kept if its distance is significantly smaller than the second best match (match[1]).
//			cout<<"Matched keypoints: "<<match[0].queryIdx<<" and "<<match[0].trainIdx<<endl;
			goodMatches.push_back(match[0]);
		}
	}
}


void printSimilarity(const std::vector<cv::DMatch> &matches, float similarityThreshold, float strongTransformThreshold){
	std::cout<<"Number of matches: "<<matches.size()<<std::endl;

	bool isSimilar = matches.size() > similarityThreshold;

	float matchesDistAcc = 0;
	for(cv::DMatch match : matches){
		matchesDistAcc += match.distance;
	}
	std::cout<<"Average distance of matches: "<<matchesDistAcc/matches.size()<<std::endl;

	bool isStrongTransform = matchesDistAcc/matches.size() > strongTransformThreshold;  // or maybe use a difference in angle between keypoints of the two images

	if(isSimilar && !isStrongTransform){
		std::cout<<"The images are similar"<<std::endl;
	}else if(isSimilar && isStrongTransform){
		std::cout<<"The images are similar and the transformation is strong"<<std::endl;
	}else{
		std::cout<<"The images are not similar"<<std::endl;
	}
}


cv::Mat bruteForceSIFTMatching(const cv::Mat &img1, const cv::Mat &img2, bool toPrintSimilarity=false){
	cv::Ptr<cv::SIFT> sift = cv::SIFT::create();

	std::vector<cv::KeyPoint> keypoints1, keypoints2;
	cv::Mat descriptors1, descriptors2;
	sift->detectAndCompute(img1, cv::noArray(), keypoints1, descriptors1);
	sift->detectAndCompute(img2, cv::noArray(), keypoints2, descriptors2);

	cv::Ptr<cv::BFMatcher> bfMatcher = cv::BFMatcher::create();

	std::vector<cv::DMatch> goodMatches;
	applyLoweRatio(bfMatcher, descriptors1, descriptors2, goodMatches);

	cv::Mat outImg;
	drawMatches(img1, keypoints1, img2, keypoints2, goodMatches, outImg, cv::Scalar::all(-1), cv::Scalar::all(-1), std::vector<char>(), cv::DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);

	if (toPrintSimilarity)
		printSimilarity(goodMatches, 50, 170);

	return outImg;
}


cv::Mat bruteForceORBMatching(const cv::Mat &img1, const cv::Mat &img2, bool toPrintSimilarity=false){
	cv::Ptr<cv::ORB> orb = cv::ORB::create();

	std::vector<cv::KeyPoint> keypoints1, keypoints2;
	cv::Mat descriptors1, descriptors2;
	orb->detectAndCompute(img1, cv::noArray(), keypoints1, descriptors1);
	orb->detectAndCompute(img2, cv::noArray(), keypoints2, descriptors2);

	cv::Ptr<cv::BFMatcher> bfMatcher = cv::BFMatcher::create(cv::NORM_HAMMING);

	std::vector<cv::DMatch> goodMatches;
	applyLoweRatio(bfMatcher, descriptors1, descriptors2, goodMatches);

	cv::Mat outImg;
	drawMatches(img1, keypoints1, img2, keypoints2, goodMatches, outImg, cv::Scalar::all(-1), cv::Scalar::all(-1), std::vector<char>(), cv::DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);


	if (toPrintSimilarity)
		printSimilarity(goodMatches, 30, 70);

	return outImg;
}