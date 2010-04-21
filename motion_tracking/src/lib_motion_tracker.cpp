#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <cv_bridge/CvBridge.h>

#include <motion_tracking/lib_motion_tracker.h>

int rlow = 60;
int rhigh = 80;
int glow = 85;
int ghigh = 120;
int blow = 70;
int bhigh = 100;

int xSize = 640;
int ySize = 480;

using namespace cv;
void blobfind(const cv::Mat& src, cv::Mat& out)
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

	vector<Point2f> corners;

	goodFeaturesToTrack(out, vector<Point2f>& corners, int maxCorners=4, double qualityLevel=.01, double minDistance=1, const Mat& mask=Mat(), int blockSize=3, bool useHarrisDetector=false, double k=0.04);

	int xCenter = (corners[0].x+corners[2].x)/2;
	int yCenter = (corners[0].y+corners[2].y)/2;

	int xOffset = xSize/2 - xCenter;
	int yOffset = ySize/2 - yCenter;
	//merge(mats, out);
	//out = mats[0];
}
