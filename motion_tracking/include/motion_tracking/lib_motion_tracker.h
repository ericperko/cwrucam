#ifndef _LIB_MOTION_TRACKER_H
#define _LIB_MOTION_TRACKER_H

#include <opencv/cv.h>

void blobfind(const cv::Mat& src, cv::Mat& out, cv::Point2i& vec);

void blobfind_hsv(const cv::Mat& src, cv::Mat& out, cv::Point2i& vec);

void mge_method(const cv::Mat& src, cv::Mat& out, cv::Point2i& vec, double timestamp);

#endif
