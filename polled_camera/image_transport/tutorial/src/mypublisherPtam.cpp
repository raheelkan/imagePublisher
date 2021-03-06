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
  //std::istringstream video_sourceCmd(camNo);
  int video_source;
  // Check if it is indeed a number
  //if(!(video_sourceCmd >> video_source)) return 1;

  cv::VideoCapture cap(1);
  cerr << endl << "Camera Started!" << endl;

  // Check if video device can be opened with the given index
  if(!cap.isOpened()) return 1;
  cv::Mat frame, frame_gray;
  sensor_msgs::ImagePtr msg;
  cerr << endl << "Publishing Frames to the topic /camera/image_raw (For PTAM)..." << endl;
  ros::Rate loop_rate(5);
  while (nh.ok()) {
    cap >> frame;
    cv::cvtColor(frame, frame_gray, CV_BGR2GRAY);
	//cout << "Width : " << frame_gray.cols << endl;
	//cout << "Height : " << frame_gray.rows << endl;
    // Check if grabbed frame is actually full with some content
    if(!frame.empty()) {
      msg = cv_bridge::CvImage(std_msgs::Header(), "mono8", frame_gray).toImageMsg();//bgr8
      pub.publish(msg);
      cv::waitKey(1);
    }

    ros::spinOnce();
    loop_rate.sleep();
  }
}
