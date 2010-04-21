#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <cv_bridge/CvBridge.h>

void imageCallback(const sensor_msgs::ImageConstPtr& msg)
{
	sensor_msgs::CvBridge bridge;
	try
	{
		cvShowImage("view", bridge.imgMsgToCv(msg, "bgr8"));
	}
	catch (sensor_msgs::CvBridgeException& e)
	{
		ROS_ERROR("Could not convert from '%s' to 'bgr8'.", msg->encoding.c_str());
	}
}

int main(int argc, char **argv)
{
	ros::init(argc, argv, "motion_tracking");
	ros::NodeHandle nh;
	cvNamedWindow("view");
	cvStartWindowThread();
	image_transport::ImageTransport it(nh);
	image_transport::Subscriber sub = it.subscribe("image", 1, imageCallback);
	ros::spin();
	cvDestroyWindow("view");
}
