/*
   Codigo para filtrar el clor rojo en una imagen con opencv en formato HSV.
*/

#include "ros/ros.h"
#include "std_msgs/String.h"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <nlohmann/json.hpp>

//#######################################################################################
//using json = nlohmann::json;        //Json ordenado
using json = nlohmann::ordered_json;   //Json no ordenado.
using namespace std;
using namespace cv;

//#######################################################################################
Mat colorFilter(const Mat& src, int hueMin, int hueMax, bool show);
json contoursToJson(vector< vector<Point> > contours, float minArea);
void dibujaContornos(Mat img, vector< vector<Point> > contours);

//#######################################################################################
int main(int argc, char** argv){

   bool DrawContours = false;
   Mat input;
   Mat mask1, mask2;
   char c = 'a';
   json yelow, blue, image;
   std_msgs::String msg;
   std::string minAreaParam;
   float minArea;
	
   ros::init(argc, argv, "yelowblueJson");
   ros::NodeHandle nh("~");
   ros::Publisher chatter_pub = nh.advertise<std_msgs::String>("boundingbox", 2);
   ros::Rate loop_rate(1);
   
   nh.param<bool>("DrawContours", DrawContours, false);
   ROS_WARN("Parameter 'DrawContours' value: %s", DrawContours ? "true" : "false");  
   
   if (nh.searchParam("MinArea", minAreaParam))
      nh.getParam(minAreaParam, minArea);
   else {
      minArea = 10000.0;
      ROS_WARN("Parameter 'MinArea' not defined.");  
   }
   ROS_WARN("Parameter 'MinArea' value: %.1f", minArea);  
	
	/*
	VideoCapture cap;
	cap.open(0);
	if ( !cap.isOpened() ) {
		cerr << "couldn't open video: 0" << endl;
		return -1;
	}
	*/

   namedWindow( "Video", WINDOW_NORMAL | WINDOW_KEEPRATIO );

   cout << "Press [space] for exit!" << endl;
   while( ros::ok() && c != ' ') {
      image.clear();
   	//cap >> input;
      input = imread("/home/mixi/catkin_ws/src/find_color/src/yelow1.jpg", IMREAD_COLOR);
      if( input.empty( ) ){
         cout << "Error en carga de imagen!" << endl;
         break;
      }
      image["imageSize"]["width"] = input.size[1];
      image["imageSize"]["height"] = input.size[0];
   
      mask1 = colorFilter(input, 210/2, 250/2, false); //Blue
      mask2 = colorFilter(input, 48/2, 58/2, false);  //Yelow
   
   	// buscar contornos en la imagen binaria
      vector< vector<Point> > contours1;
      findContours(mask1, contours1, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
      if(DrawContours)
         dibujaContornos(input, contours1);
      blue = contoursToJson(contours1, minArea);
      vector< vector<Point> > contours2;
      findContours(mask2, contours2, RETR_LIST, CHAIN_APPROX_SIMPLE);
      if(DrawContours)
         dibujaContornos(input, contours2);
      yelow = contoursToJson(contours2, minArea);
      image["rois"]["blue"] = blue.size();
      image["rois"]["yelow"] = yelow.size();
      image["blue"] = blue;
      image["yelow"] = yelow;
   	//-------------------------------------------------------
      imshow("Video", input);
      c = waitKey(100);
   	//-------------------------------------------------------
      msg.data = image.dump();
      chatter_pub.publish(msg);
      ros::spinOnce();
      loop_rate.sleep();
   }

   destroyAllWindows();

   return 0;
}

//#######################################################################################
//#######################################################################################
Mat colorFilter(const Mat& src, int hueMin, int hueMax, bool show = false){

   assert(src.type() == CV_8UC3);

   Mat mask;
   Mat img_hsv;
	
   cvtColor(src, img_hsv, COLOR_BGR2HSV);

	//mask
   inRange(img_hsv, Scalar(hueMin, 80, 80), Scalar(hueMax, 255, 255), mask);

	// Create a structuring element 
   int morph_size = 2; 
   Mat element = getStructuringElement( MORPH_RECT, 
      	Size(2 * morph_size + 1, 2 * morph_size + 1), 
      	Point(morph_size, morph_size)); 
   morphologyEx(mask, mask, MORPH_CLOSE, element, Point(-1, -1), 2); 
   morphologyEx(mask, mask, MORPH_OPEN, element, Point(-1, -1), 2); 

   if(show){
      imshow("mask", mask);
   	//imshow("img_hsv", img_hsv);
      Mat colorOnly;
      bitwise_and(src, src, colorOnly, mask);
      imshow("colorOnly", colorOnly);
   }

   return mask;
}

//#######################################################################################
json contoursToJson(vector< vector<Point> > contours, float minArea){
   json region, regions;
   float area;

	// dibujar rectangulo y texto con coordenadas (x, y)
   for (vector<Point> contour : contours) {
      Rect r = boundingRect(contour);
      area = contourArea(contour);
      if(area > minArea){
         region.clear();
         region["center"] = {r.x + (r.width / 2), r.y + (r.height / 2)};
         region["size"] = {r.width, r.height};
         region["area"] = area;
      	
         regions.push_back(region);
      }
   }
   return regions;
}

//#######################################################################################
void dibujaContornos(Mat img, vector< vector<Point> > contours){
   drawContours(img, contours, -1, Scalar(255));

	// dibujar rectangulo y texto con coordenadas (x, y)
   for (vector<Point> contour : contours) {
      Rect r = boundingRect(contour);
      rectangle(img, r.tl(), r.br(), CV_RGB(255, 0, 0), 1, LINE_AA, 0);
   
      Point center(r.x + (r.width / 2), r.y + (r.height / 2));
   
      ostringstream str;
      str << "[" << contourArea(contour) << "]" << center.x << "," << center.y;
      putText(img, str.str(), center, FONT_HERSHEY_COMPLEX_SMALL, 0.60, CV_RGB(0, 255, 0), 1, LINE_AA);
   }
}

