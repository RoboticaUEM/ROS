#include "ros/ros.h"
#include "std_msgs/Int16MultiArray.h"

#include <sstream>
#include <vector>

const int TAM_VECTOR = 10;

int main(int argc, char **argv){

	ros::init(argc, argv, "array_push");

	ros::NodeHandle n;

	ros::Publisher chatter_pub = n.advertise<std_msgs::Int16MultiArray>("array_push", 2);

	ros::Rate loop_rate(5);
  
	std::vector<int16_t> vec;
	int x = 0;
	while (ros::ok()){

		vec.resize(TAM_VECTOR);
		for (int i = 0; i < TAM_VECTOR; ++i){
			vec[i] = x;
			x++;
		}

		std_msgs::Int16MultiArray msg;

		//push data into data blob
		msg.data = vec;

		chatter_pub.publish(msg);
		ros::spinOnce();

		//clear stuff and sleep
		vec.clear();
		loop_rate.sleep();
	}

   return 0;
}
