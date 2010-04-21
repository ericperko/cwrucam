#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <cv_bridge/CvBridge.h>

int rlow = 60;
int rhigh = 80;
int glow = 85;
int ghigh = 120;
int blow = 70;
int bhigh = 100;

void blobfind(const IplImage& src, IplImage& vec)
{
	IplImage *r = cvCreateImage(cvGetSize(src), src->depth, 1);
	IplImage *g = cvClone(r);
	IplImage *b = cvClone(r);

	cvSplit(img, r, g, b, NULL);
	
	// Show green channel
	cvShowImage("Green", g);
}
