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

using namespace cv;
void blobfind(const cv::Mat& src, cv::Mat& out)
{
	vector<cv::Mat> mats;

	split(src, mats);

	// Set all values below value to zero, leave rest the same
	// Then inverse binary threshold the remaining pixels
	
	// Threshold blue channel
	threshold(mats[0], mats[0], blow, 255, 3);
	threshold(mats[0], mats[0], bhigh, 255, 1);
	// Threshold green channel
	threshold(mats[1], mats[1], glow, 255, 3);
	threshold(mats[1], mats[1], ghigh, 255, 1);
	// Threshold red channel
	threshold(mats[2], mats[2], rlow, 255, 3);
	threshold(mats[2], mats[2], rhigh, 255, 1);

	// Multiply binary images to produce location of sponge
	multiply(mats[0],mats[1],out);
	multiply(mats[2],out,out);
}
