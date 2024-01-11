#include "ros/ros.h"
#include "std_msgs/String.h"
#include <nlohmann/json.hpp>

using json = nlohmann::ordered_json;

void  jsonReceived(const std_msgs::String& msg);

int main(int argc, char** argv){
	ros::init(argc, argv, "json_parse");
	ros::NodeHandle n;
	ros::Subscriber sub = n.subscribe("yeloblueJson", 1, jsonReceived);
	ros::spin();

	return 0;
}

void  jsonReceived(const std_msgs::String& msg){

	//std::cout << std::endl << json::parse(msg.data).dump(4) << std::endl;
	ROS_WARN( "%s", json::parse(msg.data).dump(4).c_str() );

}
