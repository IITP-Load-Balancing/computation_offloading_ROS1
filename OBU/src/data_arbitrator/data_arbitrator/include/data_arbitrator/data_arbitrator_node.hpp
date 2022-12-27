#pragma once

// c++
#include <pthread.h>
#include <chrono>
#include <cmath>
#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <sys/time.h>

// ROS
#include <actionlib/server/simple_action_server.h>
#include <geometry_msgs/Point.h>
// #include <image_transport/image_transport.h>
#include <ros/ros.h>
#include <sensor_msgs/Image.h>
#include <sensor_msgs/image_encodings.h>
#include <std_msgs/Header.h>

// OpenCv
#include <cv_bridge/cv_bridge.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>

// darknet_ros_msgs
#include <darknet_ros_msgs/BoundingBox.h>
#include <darknet_ros_msgs/BoundingBoxes.h>
#include <darknet_ros_msgs/CheckForObjectsAction.h>
#include <darknet_ros_msgs/ObjectCount.h>
//#include <darknet_ros_msgs/Time.h>


namespace data_arbitrator {

//! Bounding box of the detected object.

class DataArbitrator {
 public:
  /*!
   * Constructor.
   */
  explicit DataArbitrator(ros::NodeHandle nh);

  /*!
   * Destructor.
   */
  ~DataArbitrator();

 private:
  /*!
   * Reads and verifies the ROS parameters.
   * @return true if successful.
   */
  bool readParameters();

  /*!
   * Initialize the ROS connections.
   */
  void init();

  //! ROS node handle.
  ros::NodeHandle nodeHandle_;

  //! Timer handle
  ros::Subscriber executionTimeSubscriber_;
  double execution_time;

  //! Class labels.
  int numClasses_;
  std::vector<std::string> classLabels_;


  //! Advertise and subscribe to image topics.

  //! ROS subscriber and publisher.
  //For local_yolo
  ros::Subscriber objectSubscriber_;
  ros::Subscriber boundingBoxesSubscriber_;
  //For server_yolo
  ros::Subscriber objectSubscriber2_;
  ros::Subscriber boundingBoxesSubscriber2_;


  //! Detected objects.
//   std::vector<std::vector<RosBox_> > rosBoxes_;
  std::vector<int> rosBoxCounter_;
  darknet_ros_msgs::BoundingBoxes boundingBoxesResults_;

  //! Camera related parameters.
  int frameWidth_;
  int frameHeight_;

  void objectCallback(const darknet_ros_msgs::ObjectCount& msg);
  void objectCallback2(const darknet_ros_msgs::ObjectCount& msg);
  void boxCallback(const darknet_ros_msgs::BoundingBoxes& msg);
  void boxCallback2(const darknet_ros_msgs::BoundingBoxes& msg);
//   void objectCallback(const darknet_ros_msgs::ObjectCount& msg);
};

} /* namespace darknet_ros*/
