#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <cv_bridge/CvBridge.h>

#include <motion_tracking/lib_motion_tracker.h>

#include <motion_tracking/cvblob.h>

int rlow = 60;
int rhigh = 80;
int glow = 85;
int ghigh = 120;
int blow = 70;
int bhigh = 100;

int hlow = 45;
int hhigh = 70;

int xSize = 640;
int ySize = 480;

bool initial = true;

const int diff_threshold = 30;
const double max_time_diff = 0.5;
const double min_time_diff = 0.05;
const double max_time_to_track = 1.0;

using namespace cv;
using namespace cvb;

Mat last;
Mat mhi;

void blobfind(const cv::Mat& src, cv::Mat& out, cv::Point2i& vec)
{
	Mat temp;

	//cvtColor(src, temp, CV_BGR2HSV);
	temp = src;

	vector<Mat> mats;

	split(temp, mats);

	// Set all values below value to zero, leave rest the same
	// Then inverse binary threshold the remaining pixels
	
	// Threshold blue channel
	threshold(mats[0], mats[0], bhigh, 255, THRESH_TOZERO_INV);
	threshold(mats[0], mats[0], blow, 255, THRESH_BINARY);
	// Threshold green channel
	threshold(mats[1], mats[1], ghigh, 255, THRESH_TOZERO_INV);
	threshold(mats[1], mats[1], glow, 255, THRESH_BINARY);
	// Threshold red channel
	threshold(mats[2], mats[2], rhigh, 255, THRESH_TOZERO_INV);
	threshold(mats[2], mats[2], rlow, 255, THRESH_BINARY);

	multiply(mats[0], mats[1], out);
	multiply(out, mats[2], out);

	erode(out, out, Mat());	

	dilate(out, out, Mat(), Point(-1,-1), 30);

	IplImage temp1 = out;
	IplImage temp2 = temp;
	IplImage *labelImg=cvCreateImage(cvGetSize(&temp1), IPL_DEPTH_LABEL, 1);
	CvBlobs blobs;
	unsigned int result=cvLabel(&temp1, labelImg, blobs);
	cvRenderBlobs(labelImg, blobs, &temp2, &temp2);

	CvLabel greatestBlob = cvGreaterBlob(blobs); 
	CvPoint2D64f center;
	center.x = xSize/2;
	center.y = ySize/2;
	if (greatestBlob > 0) {
	    center = cvCentroid(blobs[greatestBlob]);
	}

	vector<Point2f> corners;

	goodFeaturesToTrack(out, corners, 4,.01, 1,Mat(),3);

	//int xCenter = (corners[0].x+corners[2].x)/2;
	//int yCenter = (corners[0].y+corners[2].y)/2;

	int xOffset = xSize/2 - (int) center.x;
	int yOffset = -ySize/2 + (int) center.y;

	//std::cout << xOffset << " " << yOffset << std::endl;

	vec = Point2i(xOffset, yOffset);

	//circle(out, corners[0], 10, Scalar(255,0,0));
	//circle(out, corners[2], 10, Scalar(255,0,0));
	//merge(mats, out);
	//out = mats[0];
	out = temp;
	cvReleaseImage(&labelImg);
}

void mge_method(const cv::Mat& src, cv::Mat& out, cv::Point2i& vec, double timestamp)
{
	Mat temp;
	Mat temp2;
	Mat mask = Mat(src.size(), CV_8UC1);
	Mat orientation;

	if (initial) {
	    //First run through - don't calc any motion stuff, setup the initial Matrix

	    cvtColor(src, last, CV_RGB2GRAY);
	    mhi = Mat(src.size(), CV_32F);
	    vec = Point2i(0, 0);
	    initial = false;
	    out = last;
	}
	else {
	    cvtColor(src, temp, CV_RGB2GRAY);
	    absdiff(last, temp, temp2);
	    threshold(temp2, temp2, diff_threshold, 255, CV_THRESH_BINARY); 
	    //imshow("view", temp2);
	    updateMotionHistory(temp2, mhi, timestamp, max_time_to_track); 
	    //imshow("view", mhi);
	    std::cout << "mhi " << countNonZero(mhi) << std::endl;
	    calcMotionGradient(mhi, mask, orientation, max_time_diff, min_time_diff);
	    //imshow("view", mask);
	    std::cout << "mask " << countNonZero(mask) << std::endl;
	    out = mask;
	    last = temp;
	}
}

void blobfind_hsv(const cv::Mat& src, cv::Mat& out, cv::Point2i& vec)
{
	Mat temp;
	Mat temp_src = src;

	cvtColor(src, temp, CV_BGR2HSV);

	vector<Mat> mats;

	split(temp, mats);

	// Set all values below value to zero, leave rest the same
	// Then inverse binary threshold the remaining pixels
	
	// Threshold on hue
	threshold(mats[0], mats[0], hhigh, 255, THRESH_TOZERO_INV);
	threshold(mats[0], mats[0], hlow, 255, THRESH_BINARY);


	multiply(mats[0], mats[1], out);
	multiply(out, mats[2], out);

	erode(out, out, Mat(), Point(-1,-1), 2);	

	dilate(out, out, Mat(), Point(-1,-1), 30);

	IplImage temp1 = out;
	IplImage temp2 = temp_src;
	IplImage *labelImg=cvCreateImage(cvGetSize(&temp1), IPL_DEPTH_LABEL, 1);
	CvBlobs blobs;
	unsigned int result=cvLabel(&temp1, labelImg, blobs);
	cvRenderBlobs(labelImg, blobs, &temp2, &temp2);

	CvLabel greatestBlob = cvGreaterBlob(blobs); 
	CvPoint2D64f center;
	center.x = xSize/2;
	center.y = ySize/2;
	if (greatestBlob > 0) {
	    center = cvCentroid(blobs[greatestBlob]);
	}

	vector<Point2f> corners;

	goodFeaturesToTrack(out, corners, 4,.01, 1,Mat(),3);

	//int xCenter = (corners[0].x+corners[2].x)/2;
	//int yCenter = (corners[0].y+corners[2].y)/2;

	int xOffset = xSize/2 - (int) center.x;
	int yOffset = -ySize/2 + (int) center.y;

	//std::cout << xOffset << " " << yOffset << std::endl;

	vec = Point2i(xOffset, yOffset);

	//circle(out, corners[0], 10, Scalar(255,0,0));
	//circle(out, corners[2], 10, Scalar(255,0,0));
	//merge(mats, out);
	//out = mats[0];
	out = temp_src;
	cvReleaseImage(&labelImg);
}
