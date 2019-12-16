#include "utility.h"

void loadOrb(string file_name,pcl::PointCloud<PointType>::Ptr pcPtr,int mode);
#define READ_SUMA   1
#define READ_ORB2   2
int main(int argc, char **argv)
{

    ros::init(argc, argv, "ramTra");

    pcl::PointCloud<PointType>::Ptr fullCloud(new pcl::PointCloud<PointType>());

    pcl::PointCloud<PointType>::Ptr sumaTra(new pcl::PointCloud<PointType>());

    pcl::PointCloud<PointType>::Ptr legoTra(new pcl::PointCloud<PointType>());

    pcl::PointCloud<PointType>::Ptr ourTra(new pcl::PointCloud<PointType>());

    pcl::PointCloud<PointType>::Ptr orbTra(new pcl::PointCloud<PointType>());

    loadOrb("/media/qzj/Windows/love/code/catkin_ws/src/LeGO-LOAM/experiment/pltram/orbtra.txt",orbTra,READ_ORB2);
    sensor_msgs::PointCloud2 orbMsg;
    pcl::toROSMsg(*orbTra, orbMsg);
    orbMsg.header.frame_id = "world";

    if (pcl::io::loadPCDFile<PointType> ("/media/qzj/Windows/love/code/catkin_ws/src/LeGO-LOAM/experiment/pltram/ourTra.pcd", *ourTra) == -1)
        PCL_ERROR ("Couldn't read PCD file \n");
    sensor_msgs::PointCloud2 ourMsg;
    pcl::toROSMsg(*ourTra, ourMsg);
    ourMsg.header.frame_id = "world";

    if (pcl::io::loadPCDFile<PointType> ("/media/qzj/Windows/love/code/catkin_ws/src/LeGO-LOAM/experiment/pltram/legoTra.pcd", *legoTra) == -1)
        PCL_ERROR ("Couldn't read PCD file \n");
    sensor_msgs::PointCloud2 legoMsg;
    pcl::toROSMsg(*legoTra, legoMsg);
    legoMsg.header.frame_id = "world";

    if (pcl::io::loadPCDFile<PointType> ("/media/qzj/Windows/love/code/catkin_ws/src/LeGO-LOAM/experiment/pltram/finalCloud.pcd", *fullCloud) == -1)
        PCL_ERROR ("Couldn't read PCD file \n");
    sensor_msgs::PointCloud2 mapMsg;
    pcl::toROSMsg(*fullCloud, mapMsg);
    mapMsg.header.frame_id = "world";

    loadOrb("/media/qzj/Windows/love/code/catkin_ws/src/LeGO-LOAM/experiment/pltram/sumaTra.txt",sumaTra,READ_SUMA);
    sensor_msgs::PointCloud2 sumaMsg;
    pcl::toROSMsg(*sumaTra, sumaMsg);
    sumaMsg.header.frame_id = "world";
    
    ros::NodeHandle n;
    ros::Publisher publishOrbTra = n.advertise<sensor_msgs::PointCloud2>("/orbTraTopic", 1);
    ros::Publisher publishOurTra = n.advertise<sensor_msgs::PointCloud2>("/ourTraTopic", 1);
    ros::Publisher publishLegoTra = n.advertise<sensor_msgs::PointCloud2>("/legoTraTopic", 1);
    ros::Publisher publishSumaTra = n.advertise<sensor_msgs::PointCloud2>("/sumaTraTopic", 1);
    ros::Publisher publishMap = n.advertise<sensor_msgs::PointCloud2>("/mapTopic", 1);

    ros::Rate loop_rate(10);

    ROS_INFO("\033[1;32m---->\033[0m RAM, to be accepted !");

    int count = 0;

    while (ros::ok())
    {
        publishOrbTra.publish(orbMsg);
        publishOurTra.publish(ourMsg);
        publishLegoTra.publish(legoMsg);
        publishSumaTra.publish(sumaMsg);
        publishMap.publish(mapMsg);

        ros::spinOnce();

        loop_rate.sleep();
        ++count;
    }

    printf("RAM, accepted !");
    return 0;
}


void loadOrb(string file_name,pcl::PointCloud<PointType>::Ptr cloud,int mode)
{
    FILE *fp = fopen(file_name.c_str(),"r");
    if (!fp)
        return ;
    int count=0;
    while (!feof(fp)) {
        double data[8]= {0.f};
        if (fscanf(fp, "%lf %lf %lf %lf %lf %lf %lf %lf",&data[0], &data[1], &data[2], &data[3],&data[4], &data[5], &data[6], &data[7] )!=8)
            break;
//         cout<<data[1]<<'\t'<<data[2]<<'\t'<<data[3]<<endl;
        count++;
    }
    fclose(fp);

    cloud->width = count;
    cloud->height = 1;
    cloud->points.resize(cloud->width * cloud->height);

    FILE *fp1 = fopen(file_name.c_str(),"r");
    count=0;
    while (!feof(fp1)) {
        double data[8]= {0.f};
        if (fscanf(fp1, "%lf %lf %lf %lf %lf %lf %lf %lf",&data[0], &data[1], &data[2], &data[3],&data[4], &data[5], &data[6], &data[7] )==8)
        {
            if(mode==READ_ORB2)
            {
                double s=45.0;
                double angle=14.0;
                cloud->points[count].x = -data[1]*s-5.0;
                cloud->points[count].y = data[2]*s;
                cloud->points[count].z = data[3]*s;
                double tempx=cloud->points[count].x;
                double tempz=cloud->points[count].z;
                cloud->points[count].x=cos(angle/180.0*PI)*tempx+sin(angle/180.0*PI)*tempz;
                cloud->points[count].z=-sin(angle/180.0*PI)*tempx+cos(angle/180.0*PI)*tempz;
            }
            else if(mode==READ_SUMA)
            {
                cloud->points[count].x = data[2];
                cloud->points[count].y = data[3];
                cloud->points[count].z = data[1];
            }
        }
        else
            break;
        count++;
    }
    cout<<"orb points "<<count<<endl;
    fclose(fp1);
}
