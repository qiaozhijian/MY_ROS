#include <iostream>
#include <pcl/io/pcd_io.h>
#include <pcl/point_types.h>
#include <fstream>
#include <sstream>
#include <opencv/cv.h>
#include <opencv2/opencv.hpp>  
#include "utility.h"

using namespace std;

void bin2pcd(string pathBin,string pathPcd)
{
  pcl::PointCloud<pcl::PointXYZI> point_cloud;
  pcl::PointXYZI point;
  
  int32_t num = 10000000;
  int32_t file_size;
  float *data;
  data = (float*)malloc(num * sizeof(float));

  // pointers
  float *px = data + 0;
  float *py = data + 1;
  float *pz = data + 2;
  float *pr = data + 3;
  
  fstream a_file( pathBin.c_str() , ios::binary | ios::in | ios::ate);
  file_size = a_file.tellg();

  a_file.seekg( 0, ios::beg );

  if ( ! a_file.read( reinterpret_cast<char*>( data ), file_size ) ) {
    cout << "Error reading from file" << endl;
    return;
  }


  point_cloud.width = (file_size / sizeof(float)) / 4;
  point_cloud.height = 1;
  point_cloud.is_dense = false;
  point_cloud.points.resize(point_cloud.width * point_cloud.height);

  for (int i = 0; i < point_cloud.points.size() ; ++i) {
    point_cloud.points[i].x = *px;
    point_cloud.points[i].y = *py;
    point_cloud.points[i].z = *pz;
    point_cloud.points[i].intensity = *pr;
    // cout<< "file size: "<< file_size << "  num: "<< num<< endl<< "point: " << i<< endl;
    // point_cloud.points.push_back(point);
    px += 4; py += 4; pz += 4; pr += 4;
  }


  pcl::io::savePCDFileASCII (pathPcd, point_cloud);

  std::cerr << "Saved " << point_cloud.points.size () << " data points" << std::endl;


  delete data;
  data = NULL;    
}

int main(int argc, char const *argv[]) {

  string filename=zfill(argv[1],6);
  
  string filenameIn = "/media/qzj/Document/grow/slam/slamDataSet/kitti/data_odometry_velodyne/dataset/00/velodyne/"+ filename +".bin";

  string filenameOut="/media/qzj/Document/grow/slam/slamDataSet/kitti/data_odometry_velodyne/dataset/00/velodyne/"+ filename + ".pcd";

  bin2pcd(filenameIn,filenameOut);


  return 0;
}
