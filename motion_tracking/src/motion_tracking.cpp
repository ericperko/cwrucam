#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <cv_bridge/CvBridge.h>
#include <geometry_msgs/Twist.h>

#include <motion_tracking/lib_motion_tracker.h>

class MotionTracker {
	public:
		MotionTracker();
		void imageCallback(const sensor_msgs::ImageConstPtr& msg);
	private:
		ros::NodeHandle nh_;
		image_transport::ImageTransport it_;
		image_transport::Subscriber sub_;
		image_transport::Publisher image_pub_;
		ros::Publisher twist_pub_;

		double pan_, tilt_;
};

MotionTracker::MotionTracker() : 
	it_(nh_), pan_(0.012), tilt_(0.012) 
{
	nh_.param("pan", pan_, pan_);
	nh_.param("tilt", tilt_, tilt_);
	sub_ = it_.subscribe("image", 1, &MotionTracker::imageCallback, this);
	image_pub_ = it_.advertise("motion_tracking_image", 1);
	twist_pub_ = nh_.advertise<geometry_msgs::Twist>("cmd_vel", 1);
}

void MotionTracker::imageCallback(const sensor_msgs::ImageConstPtr& msg)
{
	sensor_msgs::CvBridge bridge;
	cv::Mat image;
	cv::Mat output;
	cv::Point2i vec;
	geometry_msgs::Twist twist;
	try
	{
		image = cv::Mat(bridge.imgMsgToCv(msg, "bgr8"));

	}
	catch (sensor_msgs::CvBridgeException& e)
	{
		ROS_ERROR("Could not convert from '%s' to 'bgr8'.", msg->encoding.c_str());
	}
	try {
		mge_method(image, output, vec, ros::Time::now().toSec());
		//cv::imshow("view", output);
		IplImage temp = output;
		image_pub_.publish(bridge.cvToImgMsg(&temp, "mono8"));
	}
	catch (sensor_msgs::CvBridgeException& e) {
	    
		ROS_ERROR("Could not convert from '%s' to 'mono8'.", msg->encoding.c_str());
	}
	//std::cout << vec.x << " " << vec.y << std::endl;
	twist.angular.z = vec.x * pan_;
	twist.angular.y = vec.y * tilt_;
	twist_pub_.publish(twist);
}

int main(int argc, char **argv)
{
	ros::init(argc, argv, "motion_tracking");
	MotionTracker motion_tracker;
	cvNamedWindow("view");
	cvStartWindowThread();
	ros::spin();
	cvDestroyWindow("view");
}
