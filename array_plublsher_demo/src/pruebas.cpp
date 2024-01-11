#include "ros/ros.h"
#include "std_msgs/Int16MultiArray.h"

#include <sstream>
#include <vector>

int main(int argc, char **argv){

	ros::init(argc, argv, "publicArray");

	ros::NodeHandle n;

	ros::Publisher chatter_pub = n.advertise<std_msgs::Int16MultiArray>("array", 2);

	ros::Rate loop_rate(5);
  
	std::vector<int16_t> vec;
	int x = 0;
	while (ros::ok()){
		std_msgs::Int16MultiArray msg;

		for (int i = 0; i < 5; ++i){
			vec.push_back(x);
			x++;
		}

		msg.layout.dim.push_back(std_msgs::MultiArrayDimension());  
		msg.layout.dim[0].size = vec.size();
		msg.layout.dim[0].stride = 1;
		msg.layout.dim[0].label = std::string("V");

		msg.data.resize(2);
		//push data into data blob
		std::vector<int16_t>::const_iterator itr, end(vec.end());
		for(itr = vec.begin(); itr!= end; ++itr) {
		//cout<<*itr<<endl;
		//msg.data.push_back(*itr);	//-> esta funciona
		msg.data[0] = vec[0];			//-> esta tambien
		}

		chatter_pub.publish(msg);
		ros::spinOnce();

		//clear stuff and sleep
		vec.clear();
		msg.data.clear();
		loop_rate.sleep();
	}

   return 0;
}
