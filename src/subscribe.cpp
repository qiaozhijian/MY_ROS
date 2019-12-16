#include "utility.h"


void chatterCallback(const std_msgs::String::ConstPtr& msg)
{ 
  ROS_INFO("I heard: [%s]", msg->data.c_str());
  ros::Duration(0.05).sleep();
}
void chatterCallback2(const std_msgs::String::ConstPtr& msg)
{
  ROS_INFO("2 I heard: [%s]", msg->data.c_str());
}
int main(int argc, char **argv)
{
    
  ros::init(argc, argv, "subscribe");

  ROS_INFO("\033[1;32m---->\033[0m subscribe Started.");
  
  ros::NodeHandle n;
  ros::Subscriber sub = n.subscribe("chatter", 1, chatterCallback);
  ros::Subscriber sub2 = n.subscribe("chatter2", 1, chatterCallback2);

  ros::spin();
  
  printf("subscribe over");
  return 0;
}
