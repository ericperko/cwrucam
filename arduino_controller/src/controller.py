#!/usr/bin/env python

__author__="Eric Perko (exp63)"

import serial

import roslib
roslib.load_manifest('arduino_controller')
import rospy
from geometry_msgs.msg import Twist

def twist_receiver(msg, port):
    command_string = "%d %d\r" % (-1*msg.angular.z, -1*msg.angular.y)
    port.write(command_string)

if __name__ == "__main__":
	port = serial.Serial('/dev/ttyUSB0', 115200)
	rospy.init_node('arduino_controller')
	rospy.Subscriber('cmd_vel', Twist, twist_receiver, port)
	rospy.spin()
	port.close()
