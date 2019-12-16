#include "utility.h"

using std::cout;
using std::endl;
using std::string;
using std::ofstream;
using std::stringstream;
using std::fixed;
using std::setprecision;
ofstream time_file;
string filepath;
int count1=0;

string int2string(int i,int num)
{
    std::string str = std::to_string(i);
    while (str.size() < num)
        str = "0" + str;
    return str;
}
double startTime=0.0;
double endTime=90.0;
// int gap=10;
pcl::PointCloud<PointType>::Ptr laserCloud(new pcl::PointCloud<PointType>());

void bin_callback ( const sensor_msgs::PointCloud2ConstPtr& msg )
{
    static double tStart=0.0;
    double t=msg->header.stamp.toSec();
    laserCloud->clear();
    pcl::fromROSMsg(*msg, *laserCloud);

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

    //convert to bin
    string name=string ( int2string(count1,6) +".bin" );
    count1++;
//     if((count1-1)%gap==0)
    {
        std::ofstream binFile(filepath+"/bin/"+name);
        cout<<name<<endl;
        for (size_t i = 0; i < laserCloud->points.size (); ++i)
            binFile << laserCloud->points[i].x << '\t' << laserCloud->points[i].y << '\t' << laserCloud->points[i].z << "\t" << 0 <<std::endl;
        binFile.close();
    }
}
bool checkPath ( std::string &path )
{
    DIR *dir;
    int isCreate=0 ;
    if ( ( dir=opendir ( ( path+"/bin/" ).c_str() ) ) == NULL ) {
        isCreate =mkdir ( ( path+"/bin/" ).c_str(),S_IRUSR | S_IWUSR | S_IXUSR | S_IRWXG | S_IRWXO );
    }
    if ( isCreate==0 )
        return true;
}
int main(int argc, char **argv)
{
    if ( argc<2 ) {
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
    string file ( filepath+"/timestamp_bin.txt" );
    //topic
    string topic;
    topic.assign ( argv[2] );
    time_file.open ( file.c_str(),std::ios::app );

    ros::init ( argc, argv, "rosbag2bin" );
    ros::NodeHandle n;

    cout<<endl<<"topic:"<<topic.c_str() <<endl;
    cout<<"Writing bin to "<<filepath +"/bin/" <<endl;

    ros::Subscriber sub_image_imu=n.subscribe ( topic.c_str(),100,bin_callback );
    ros::Rate loop_rate ( 50 );
    while ( ros::ok() ) {
        ros::spinOnce();
        loop_rate.sleep();
    }
    time_file.close();
    cout<<"the end"<<endl;
    return 0;
}

