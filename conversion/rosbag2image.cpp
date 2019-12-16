#include "utility.h"
#include <ros/ros.h>
#include <sensor_msgs/Image.h>
#include<sensor_msgs/Imu.h>
#include<std_msgs/String.h>
#include<sstream>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
//file
#include <sys/stat.h>
#include <sys/types.h>
#include<dirent.h>
#include <string>
using std::cout;
using std::endl;
using std::string;
using std::ofstream;
using std::stringstream;
using std::fixed;
using std::setprecision;
ofstream time_file;
string filepath;
bool isColor;
std::vector<int> compression_params;
int count1=0;

string int2string(int i,int num)
{       
    std::string str = std::to_string(i);
    while (str.size() < num) 
        str = "0" + str;
    return str;
}
double startTime=60.0;
double endTime=90.0;
void imageLeft_callback ( const sensor_msgs::ImageConstPtr &img_msg )
{
    static double tStart=0.0;
    double t=img_msg->header.stamp.toSec();
    //convert to ns
    if(tStart==0.0)
        tStart=t;
    
    if(t-tStart<startTime || t-tStart>endTime)
        return;
    
    time_file<<fixed;
    if ( time_file.is_open() ) {
        //ROS_INFO("writing image timestamp");
        cout<<"timestamp:"<<t-tStart<<endl;
        time_file<<setprecision ( 19 ) <<t<<setprecision ( 0 ) <<endl;
    } else {
        ROS_WARN ( "Can not open files" );
    }
    //convert to image
    cv_bridge::CvImagePtr cv_ptr;
    try {
        if ( isColor )
            cv_ptr = cv_bridge::toCvCopy ( img_msg,sensor_msgs::image_encodings::RGB8 );
        else
            cv_ptr = cv_bridge::toCvCopy ( img_msg,sensor_msgs::image_encodings::MONO8 );
    } catch ( cv_bridge::Exception& e ) {
        ROS_ERROR ( "Could not convert from '%s' to 'mono8'.", img_msg->encoding.c_str() );
    }
    string name=string ( int2string(count1,6) +".png" );
    count1++;
    string image_left_file ( filepath+"/left/"+name );
    cout<<name<<endl;
    cv::imwrite ( image_left_file,cv_ptr->image,compression_params );
}
bool checkPath ( std::string &path )
{
    DIR *dir;
    int isCreate=0 ;
    int isC=0;
    if ( ( dir=opendir ( ( path+"/left/" ).c_str() ) ) == NULL ) {
        isCreate =mkdir ( ( path+"/left/" ).c_str(),S_IRUSR | S_IWUSR | S_IXUSR | S_IRWXG | S_IRWXO );
    }
    if ( ( dir=opendir ( ( path+"/right/" ).c_str() ) ) == NULL ) {
        isC=mkdir ( ( path+"/right/" ).c_str(),S_IRUSR | S_IWUSR | S_IXUSR | S_IRWXG | S_IRWXO );
    }

    if ( isC==0&&isCreate==0 )
        return true;
    else
        return false;
}

int main ( int argc, char **argv )
{
    if ( argc<3 ) {
        cout<<"please input file output path, topic and isColor."<<endl;
        return -1;
    }
    //path 图片放置目录
    filepath.assign ( argv[1] );
    bool isOK=checkPath ( filepath );
    if ( !isOK ) {
        cout<<"file path can not be found."<<endl;
        return -1;
    }
    string file ( filepath+"/timestamp.txt" );
    //topic
    string topic;
    topic.assign ( argv[2] );
    time_file.open ( file.c_str(),std::ios::app );
    compression_params.push_back ( cv::IMWRITE_PNG_COMPRESSION );
    compression_params.push_back ( 0 );

    std::istringstream ( argv[3] ) >>std::boolalpha>>isColor;

    ros::init ( argc, argv, "imWriteLeft" );
    ros::NodeHandle n;

    cout<<endl<<"topic:"<<topic.c_str() <<endl;
    cout<<"Writing image to "<<filepath.c_str() <<endl;
    cout<<"isColor:"<<argv[3]<<endl;

    ros::Subscriber sub_image_imu=n.subscribe ( topic.c_str(),100,imageLeft_callback );
    ros::Rate loop_rate ( 50 );
    while ( ros::ok() ) {
        ros::spinOnce();
        loop_rate.sleep();
    }
    time_file.close();
    cout<<"the end"<<endl;
    return 0;

}








