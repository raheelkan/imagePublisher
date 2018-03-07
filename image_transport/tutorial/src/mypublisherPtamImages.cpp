

#include<iostream>
#include<algorithm>
#include<fstream>
#include<chrono>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include<opencv2/core/core.hpp>

//#include<System.h>
#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <opencv2/highgui/highgui.hpp>
#include <cv_bridge/cv_bridge.h>
#include <sstream> // for converting the command line parameter to integer

#include <opencv2/imgproc/imgproc.hpp>


using namespace std;
using namespace cv;

void LoadImages(const string &strFile, vector<string> &vstrImageFilenames,
                vector<double> &vTimestamps);


int main(int argc, char **argv)
{

  ros::init(argc, argv, "image_publisher");
  cerr << endl << "ROS  initialized!" << endl;

  ros::NodeHandle nh;
  image_transport::ImageTransport it(nh);
  image_transport::Publisher pub = it.advertise("/camera/image_raw", 1);


  //  if(argc != 4)
  //  {
  //      cerr << endl << "Usage: rosrun image_transport_tutorial mypublisherPtam path_to_sequence" << endl;
  //      return 1;
  //  }

    // Retrieve paths to images
    //vector<string> vstrImageFilenames;
    //vector<double> vTimestamps;
    //string strFile = string(argv[1])+"/rgb.txt";
    //LoadImages(strFile, vstrImageFilenames, vTimestamps);

    string folder = string(argv[1])+"/*.png";
    //string suffix = ".jpg";
    vector<String> imageNames;
    glob(folder, imageNames);
    int nImages = imageNames.size(); //vstrImageFilenames.size();

    cout << endl << "-------" << endl;
    cout << "Start processing sequence ..." << endl;
    cout << "Images in the sequence: " << nImages << endl << endl;
    //cout << "Argv[1]:      " <<argv[1]<<endl<<endl;
    //cout << "vstrImageFilenames[1]      " <<vstrImageFilenames[1]<<endl<<endl;
    //cout << "vTimestamps[1]:      " <<vTimestamps[1]<<endl<<endl;

    // Main loop
    //std::stringstream ss;
    string imageName; //, number;
    cv::Mat im, frame;
    cerr << endl << "Publishing (Grayscale)Frames to the topic /camera/image_raw (For PTAM)...from: " << argv[1]<< endl;
    uint ni = 0;
    //for(int ni=0; ni<nImages; ni++)
    //{
        // Read image from file
	
	cv::Mat frame_gray;//frame
	
	sensor_msgs::ImagePtr msg;
	ros::Rate loop_rate(5);//5
	//ss << std::setw(4) << std::setfill('0');
	while (nh.ok() && ni<imageNames.size()) {
	  //cap >> frame;
	    //ss << ni; // 0000, 0001, 0002, etc...
	    //number = ss.str();
	    imageName = imageNames[ni];
        
	    frame = cv::imread(imageName);
	    //cerr << endl << "Read successful for: " << imageName<< endl; 
	    cv::cvtColor(frame, frame_gray, CV_BGR2GRAY);
	    //cerr << endl << "RGB2Gray successful for: " << imageName<< endl; 
            //double tframe = vTimestamps[ni];

            //if(frame.empty())
            //{
            //    cerr << endl << "Failed to load image at: "
            //     << imageName << endl;
            //    return 1;	
            //}
	     
	    // Check if grabbed frame is actually full with some content
	    if(!frame.empty()) {
	    	//cerr << endl << "Now Publishing:  " << imageName<< endl;
	        msg = cv_bridge::CvImage(std_msgs::Header(), "mono8", frame_gray).toImageMsg();//bgr8
	        
	        //std::string frame_id(std_msgs::Header().frame_id);
	        //cerr << "FrameID: "<< frame_id;
	        pub.publish(msg);
	        cv::waitKey(1);
		//std::cin.ignore();
		//sleep(2);
	    }
          ni++;  
	  //cerr << endl << "ni Published:  " << imageName<< endl;
	  ros::spinOnce();
	  loop_rate.sleep();
	}
    //}

    return 0;
}


void LoadImages(const string &strFile, vector<string> &vstrImageFilenames, vector<double> &vTimestamps)
{
    ifstream f;
    f.open(strFile.c_str());

    // skip first three lines
    string s0;
    getline(f,s0);
    getline(f,s0);
    getline(f,s0);

    while(!f.eof())
    {
        string s;
        getline(f,s);
        if(!s.empty())
        {
            stringstream ss;
            ss << s;
            double t;
            string sRGB;
            ss >> t;
            vTimestamps.push_back(t);
            ss >> sRGB;
            vstrImageFilenames.push_back(sRGB);
        }
    }
}

