#include "utility.h"

void blocking(void);
ros::Time startLast;
int main(int argc, char **argv)
{

  ros::init(argc, argv, "advertise");

  ros::NodeHandle n;

  ros::Publisher chatter_pub = n.advertise<std_msgs::String>("chatter", 1);
  ros::Publisher chatter_pub2 = n.advertise<std_msgs::String>("chatter2", 1);

  ros::Rate loop_rate(10);
  ROS_INFO("\033[1;32m---->\033[0m advertise Started.");

  int count = 0;
  startLast = ros::Time::now();
  while (ros::ok())
  {

    std_msgs::String msg;

    std::stringstream ss;
    ss << "hello world " << count;
    msg.data = ss.str();

    ROS_INFO("%s", msg.data.c_str());
    chatter_pub.publish(msg);
    chatter_pub2.publish(msg);
//     blocking();
//     触发回调函数，只一次，不阻塞，而spin阻塞
    ros::spinOnce();

    loop_rate.sleep();
    ++count;
  }
    
  printf("advertise over");
  return 0;
}
void blocking(void)
{
    static int count = 0;
    count ++;
    ros::Time start = ros::Time::now();
    cout << "循环时间：" << start - startLast << endl << endl;
    startLast = start;
    if (count<10)
        ros::Duration(1).sleep();                              //程序延时5s
    ros::Time stop = ros::Time::now();
    
//     cout << "总耗时：" << stop - start << endl << endl;
}
