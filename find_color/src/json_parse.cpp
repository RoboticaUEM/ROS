#include "ros/ros.h"
#include "std_msgs/String.h"
#include <nlohmann/json.hpp>

using json = nlohmann::ordered_json;

void  jsonReceived(const std_msgs::String& msg);

int main(int argc, char** argv){
   ros::init(argc, argv, "json_parse");
   ros::NodeHandle n("~");
   ros::Subscriber sub = n.subscribe("/yelowblueJson/boundingbox", 1, jsonReceived);
   ros::spin();

   return 0;
}

void  jsonReceived(const std_msgs::String& msg){

   json info = json::parse(msg.data);
	//std::cout << std::endl << info.dump(4) << std::endl;
	
   std::cout << "Image Size: " << info["imageSize"]["width"] << "," << info["imageSize"]["height"] << std::endl;
   std::cout << "rois -> blue:  " << info["rois"]["blue"] << std::endl;
   std::cout << "rois -> yelow: " << info["rois"]["yelow"] << std::endl;
	
	//Dos formas distintas para acceder a un array
   for (auto it : info["blue"]){
   	// "it" is of type json::reference and has no key() member
      std::cout << "blue: " << it << std::endl;
   }
   for (auto& el : info["yelow"].items()){
      std::cout << "yelow: " << el.key() << ".- " << el.value() << std::endl;
   }

   std::cout << "==========================================================" << std::endl;
	//ROS_WARN( "%s", info.dump(4).c_str() );

}
