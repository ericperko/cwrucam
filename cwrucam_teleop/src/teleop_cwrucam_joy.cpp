#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <joy/Joy.h>

class TeleopCWRUcam {
	public: 
		TeleopCWRUcam();
	private:
		void joyCallback(const joy::Joy::ConstPtr& joy);

		ros::NodeHandle nh_;

		int theta_axis_, psi_axis_;
		double theta_, psi_;
		ros::Publisher vel_pub_;
		ros::Subscriber joy_sub_;
};

TeleopCWRUcam::TeleopCWRUcam(): 
	theta_(1), 
	psi_(1), 
	theta_axis_(1), 
	psi_axis_(0)
{
	nh_.param("theta_axis", theta_axis_, theta_axis_);
	nh_.param("psi_axis", psi_axis_, psi_axis_);
	nh_.param("theta_", theta_, theta_);
	nh_.param("psi_", psi_, psi_);

	vel_pub_ = nh_.advertise<geometry_msgs::Twist>("cmd_vel", 1); 
	joy_sub_ = nh_.subscribe<joy::Joy>("joy", 10, &TeleopCWRUcam::joyCallback, this);
}

void TeleopCWRUcam::joyCallback(const joy::Joy::ConstPtr& joy) {
	geometry_msgs::Twist twist;
	twist.angular.y = theta_ * joy->axes[theta_axis_];
	twist.angular.z = psi_ * joy->axes[psi_axis_];
	vel_pub_.publish(twist);
}

int main(int argc, char * argv[]) {
	ros::init(argc, argv, "teleop_cwrucam");
	TeleopCWRUcam teleop_cwrucam;

	ros::spin();
}

