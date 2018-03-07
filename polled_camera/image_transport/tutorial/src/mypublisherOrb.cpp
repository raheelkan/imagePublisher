#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <opencv2/highgui/highgui.hpp>
#include <cv_bridge/cv_bridge.h>
#include <sstream> // for converting the command line parameter to integer

#include <opencv2/imgproc/imgproc.hpp>
#include <libusb-1.0/libusb.h>

using namespace std;
int main(int argc, char** argv)
{
  // Check if video source has been passed as a parameter
  //if(argv[1] == NULL) return 1;

  ros::init(argc, argv, "image_publisher");
  cerr << endl << "ROS  initialized!" << endl;

  ros::NodeHandle nh;
  image_transport::ImageTransport it(nh);
  image_transport::Publisher pub = it.advertise("/camera/image_raw", 1);

  // Convert the passed as command line parameter index for the video device to an integer
  //std::istringstream video_sourceCmd(argv[1]);
  //int in = (int)string(argv[1]);
  //int video_source = in-65;
  // Check if it is indeed a number
  //if(!(video_sourceCmd >> video_source)) return 1;
 //string in = string(argv[1]);
 // int devNo;
 // if (!in.compare('dev1')){
//	devNo = 1;
  //} else if (!in.compare('dev2')){
//	devNo = 2;
  //} else if (!in.compare('dev3')){
//	devNo = 3;
  //} else if (!in.compare('dev4')){
//	devNo = 4;
  //} else {
	//devNo = 0;
  //}
int vid = 1;
  cv::VideoCapture cap(vid);
  cerr << endl << "Camera Started:" << endl;

  // Check if video device can be opened with the given index
  if(!cap.isOpened()) return 1;
  cv::Mat frame;//, frame_gray;
  sensor_msgs::ImagePtr msg;
  cerr << endl << "Publishing Frames to the topic /camera/image_raw (For ORBSLAM)..." << endl;
  ros::Rate loop_rate(5);
  while (nh.ok()) {
    cap >> frame;
    //cv::cvtColor(frame, frame_gray, CV_BGR2GRAY);
    // Check if grabbed frame is actually full with some content
    if(!frame.empty()) {
      msg = cv_bridge::CvImage(std_msgs::Header(), "bgr8", frame).toImageMsg();//mono8
      pub.publish(msg);
      cv::waitKey(1);
    }

    ros::spinOnce();
    loop_rate.sleep();
  }
}
