#include <ros/ros.h>
#include "data_arbitrator/data_arbitrator_node.hpp"
#include <iostream>

using namespace std;

int server;
int local;

float server_cnt_num =0;
float server_total_num = 0;
float local_cnt_num =0;
float local_total_num = 0;

float server_cnt_b =0;
float local_cnt_b =0;

float last_img_cnt = 0;

ros::Time start;

int main(int argc, char** argv) {
  ros::init(argc, argv, "data_arbitrator_node");
  ros::NodeHandle node;
  data_arbitrator::DataArbitrator DataArbitrator(node);

  ros::spin();
  return 0;
}

namespace data_arbitrator {

DataArbitrator::DataArbitrator(ros::NodeHandle nh)
    : nodeHandle_(nh) {
  ROS_INFO("[DataArbitrator] Node started.");

  // Read parameters from config file.
  if (!readParameters()) {
    ros::requestShutdown();
  }

  init();
}

DataArbitrator::~DataArbitrator() {
  {
    // boost::unique_lock<boost::shared_mutex> lockNodeStatus(mutexNodeStatus_);
    // isNodeRunning_ = false;
  }
}

bool DataArbitrator::readParameters() {
  // Load common parameters.
//   nodeHandle_.param("image_view/enable_opencv", viewImage_, true);
//   nodeHandle_.param("image_view/wait_key_delay", waitKeyDelay_, 3);
//   nodeHandle_.param("image_view/enable_console_output", enableConsoleOutput_, false);

//   // Check if Xserver is running on Linux.
//   if (XOpenDisplay(NULL)) {
//     // Do nothing!
//     ROS_INFO("[YoloObjectDetector] Xserver is running.");
//   } else {
//     ROS_INFO("[YoloObjectDetector] Xserver is not running.");
//     viewImage_ = false;
//   }

//   // Set vector sizes.
//   nodeHandle_.param("yolo_model/detection_classes/names", classLabels_, std::vector<std::string>(0));
//   numClasses_ = classLabels_.size();
//   rosBoxes_ = std::vector<std::vector<RosBox_> >(numClasses_);
//   rosBoxCounter_ = std::vector<int>(numClasses_);

  return true;
}

void DataArbitrator::init() {
  ROS_INFO("[DataArbitrator] init().");

  // Initialize publisher and subscriber.
  //For local_yolo
  std::string objectDetectorTopicName;
  int objectDetectorQueueSize;
  std::string boundingBoxesTopicName;
  int boundingBoxesQueueSize;
  std::string detectionImageTopicName;
  int detectionImageQueueSize;
  //For server_yolo
  std::string objectDetectorTopicName2;
  std::string boundingBoxesTopicName2;
  std::string detectionImageTopicName2;


  //For server_yolo
  nodeHandle_.param("subscribers/object_detector/topic", objectDetectorTopicName, std::string("/darknet_ros/found_object"));
  nodeHandle_.param("subscribers/object_detector/queue_size", objectDetectorQueueSize, 1);
  nodeHandle_.param("subscribers/bounding_boxes/topic", boundingBoxesTopicName, std::string("/darknet_ros/bounding_boxes"));
  nodeHandle_.param("subscribers/bounding_boxes/queue_size", boundingBoxesQueueSize, 1);
  nodeHandle_.param("subscribers/detection_image/topic", detectionImageTopicName, std::string("/darknet_ros/detection_image"));
  nodeHandle_.param("subscribers/detection_image/queue_size", detectionImageQueueSize, 1);
  //For local_yolo
  nodeHandle_.param("subscribers/object_detector/topic", objectDetectorTopicName2, std::string("/darknet_ros2/found_object"));
  nodeHandle_.param("subscribers/bounding_boxes/topic", boundingBoxesTopicName2, std::string("/darknet_ros2/bounding_boxes"));
  nodeHandle_.param("subscribers/detection_image/topic", detectionImageTopicName, std::string("/darknet_ros2/detection_image"));

  
  //For local_yolo
  objectSubscriber_ = nodeHandle_.subscribe(objectDetectorTopicName, objectDetectorQueueSize, &DataArbitrator::objectCallback, this);
  boundingBoxesSubscriber_ = nodeHandle_.subscribe(boundingBoxesTopicName, boundingBoxesQueueSize, &DataArbitrator::boxCallback, this);
  //For server_yolo
  objectSubscriber2_ = nodeHandle_.subscribe(objectDetectorTopicName2, objectDetectorQueueSize, &DataArbitrator::objectCallback2, this);
  boundingBoxesSubscriber2_ = nodeHandle_.subscribe(boundingBoxesTopicName2, boundingBoxesQueueSize, &DataArbitrator::boxCallback2, this);
  start = ros::Time::now();
  
  
}


void DataArbitrator::objectCallback(const darknet_ros_msgs::ObjectCount& msg) {
  server_cnt_num +=1;
  server_total_num += msg.count; 
  //printf("serverObj: %f, %f, %f\n", server_cnt_num, server_total_num, server_total_num/server_cnt_num);

  return;
}

void DataArbitrator::objectCallback2(const darknet_ros_msgs::ObjectCount& msg) {
  local_cnt_num +=1;
  local_total_num += msg.count; 
  //printf("localObj: %f, %f, %f\n", local_cnt_num, local_total_num, local_total_num/local_cnt_num);

  return;
}




void DataArbitrator::boxCallback(const darknet_ros_msgs::BoundingBoxes& msg) {
  //ROS_DEBUG("[DataArbitrator] BoundingBoxes received.");
  if (last_img_cnt > msg.image_header.seq) return;
  else last_img_cnt = msg.image_header.seq;
  //server_cnt_b += 1;
  cout << "serverEnd " << msg.image_header.seq<< " " << msg.header.seq << " " << ros::Time::now() << endl;
  //printf("header: %d, %d", msg.header.seq, msg.header.stamp);
  

  return;
}

void DataArbitrator::boxCallback2(const darknet_ros_msgs::BoundingBoxes& msg) {
  //ROS_DEBUG("[DataArbitrator] BoundingBoxes received.");
  //local_cnt_b += 1;
  if (last_img_cnt > msg.image_header.seq) return;
  else last_img_cnt = msg.image_header.seq;
  cout << "localEnd " << msg.image_header.seq<< " " << msg.header.seq << " " << ros::Time::now() << endl;
  //printf("header: %d, %d", msg.header.seq, msg.header.stamp);
  

  return;
}

// void DataArbitrator::objectCallback(const darknet_ros_msgs::ObjectCount& msg) {
//   ROS_DEBUG("[DataArbitrator] Object received.");
//   ROS_INFO("[DataArbitrator] Object received: Object count %d", msg->count);

//   return;
// }
}
