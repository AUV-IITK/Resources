#include <ros/ros.h>
    #include <image_transport/image_transport.h>
    #include <opencv2/highgui/highgui.hpp>
    #include <cv_bridge/cv_bridge.h>
 #include<cv.h>
#include <highgui.h>
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv/highgui.h>
#include <dynamic_reconfigure/server.h>
#include <beginners_tutorials/dynamicConfig.h>


double decontrast=0; 

void callback(beginners_tutorials::dynamicConfig &config, uint32_t level) {
ROS_INFO("%f",config.double_param);
  decontrast=config.double_param; 

}

    void imageCallback(const sensor_msgs::ImageConstPtr& msg)
    {
      try
      {
      dynamic_reconfigure::Server<beginners_tutorials::dynamicConfig> server;
      dynamic_reconfigure::Server<beginners_tutorials::dynamicConfig>::CallbackType f;

      f = boost::bind(&callback, _1, _2);
      server.setCallback(f);
	cv::Mat img;
       cv::imshow("view", cv_bridge::toCvShare(msg, "bgr8")->image);
       cv_bridge::toCvShare(msg, "bgr8")->image.convertTo(img,-1,decontrast/50,0);
       cv::imshow("Contrast",img);
       cv::waitKey(30);
     }
     catch (cv_bridge::Exception& e)
     {
       ROS_ERROR("Could not convert from '%s' to 'bgr8'.", msg->encoding.c_str());
     }
   }
   
   int main(int argc, char **argv)
   {
     ros::init(argc, argv, "image_listener");
     ros::NodeHandle nh;
     cv::namedWindow("view");
     cv::startWindowThread();
     image_transport::ImageTransport it(nh);
    image_transport::Subscriber sub = it.subscribe("camera/image", 1, imageCallback);
     ros::spin();
     cv::destroyWindow("view");
   }
