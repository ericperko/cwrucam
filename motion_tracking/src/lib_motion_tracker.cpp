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

void blobfind(const cv::Mat& src, cv::Mat& out)
{
	cv::vector<cv::Mat> mats;

	cv::split(src, mats);
	
	// Show green channel
	out = mats[1];
}
