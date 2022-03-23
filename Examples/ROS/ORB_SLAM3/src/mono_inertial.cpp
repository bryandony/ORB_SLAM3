/**
* This file is part of ORB-SLAM3
*
* Copyright (C) 2017-2021 Carlos Campos, Richard Elvira, Juan J. Gómez Rodríguez, José M.M. Montiel and Juan D. Tardós, University of Zaragoza.
* Copyright (C) 2014-2016 Raúl Mur-Artal, José M.M. Montiel and Juan D. Tardós, University of Zaragoza.
*
* ORB-SLAM3 is free software: you can redistribute it and/or modify it under the terms of the GNU General Public
* License as published by the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* ORB-SLAM3 is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
* the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along with ORB-SLAM3.
* If not, see <http://www.gnu.org/licenses/>.
*/

#include<iostream>
#include<algorithm>
#include<fstream>
#include<chrono>
#include<vector>
#include<queue>
#include<thread>
#include<mutex>

#include "rclcpp/rclcpp.hpp"
#include<cv_bridge/cv_bridge.h>
#include "sensor_msgs/msg/imu.hpp"
#include "sensor_msgs/msg/image.hpp"

#include "opencv2/core/core.hpp"

#include"../../../../include/System.h"
#include"../../../../include/ImuTypes.h"

using namespace std;
using std::placeholders::_1;

class ImuGrabber
{
public:
    ImuGrabber(){};
    void GrabImu(const sensor_msgs::msg::Imu::ConstPtr imu_msg);

    queue<sensor_msgs::msg::Imu::ConstPtr> imuBuf;
    std::mutex mBufMutex;
};

class ImageGrabber
{
public:
    ImageGrabber(ORB_SLAM3::System* pSLAM, ImuGrabber *pImuGb, const bool bClahe): mpSLAM(pSLAM), mpImuGb(pImuGb), mbClahe(bClahe){}

    void GrabImage(const sensor_msgs::msg::Image::ConstPtr msg);
    cv::Mat GetImage(const sensor_msgs::msg::Image::ConstPtr img_msg);
    void SyncWithImu();

    queue<sensor_msgs::msg::Image::ConstPtr> img0Buf;
    std::mutex mBufMutex;
   
    ORB_SLAM3::System* mpSLAM;
    ImuGrabber *mpImuGb;

    const bool mbClahe;
    cv::Ptr<cv::CLAHE> mClahe = cv::createCLAHE(3.0, cv::Size(8, 8));
};



int main(int argc, char **argv)
{
  rclcpp::init(argc, argv);
  auto node = rclcpp::Node::make_shared("Mono_Inertial");
  //rclcpp::console::set_logger_level(ROSCONSOLE_DEFAULT_NAME, rclcpp::console::levels::Info);
  auto ret = rcutils_logging_set_logger_level(
    node->get_logger().get_name(), RCUTILS_LOG_SEVERITY_INFO);
  if (ret != RCUTILS_RET_OK) {
    RCLCPP_ERROR(node->get_logger(), "Error setting severity: %s", rcutils_get_error_string().str);
    rcutils_reset_error();
  }
  bool bEqual = false;
  if(argc < 3 || argc > 4)
  {
    cerr << endl << "Usage: rosrun ORB_SLAM3 Mono_Inertial path_to_vocabulary path_to_settings [do_equalize]" << endl;
    rclcpp::shutdown();
    return 1;
  }


  if(argc==4)
  {
    std::string sbEqual(argv[3]);
    if(sbEqual == "true")
      bEqual = true;
  }

  // Create SLAM system. It initializes all system threads and gets ready to process frames.
  ORB_SLAM3::System SLAM(argv[1], argv[2], ORB_SLAM3::System::IMU_MONOCULAR, true);

  ImuGrabber imugb;
  ImageGrabber igb(&SLAM,&imugb,bEqual); // TODO
  
  // Maximum delay, 5 seconds
  //rclcpp::Subscriber sub_imu = n.subscribe("/imu", 1000, &ImuGrabber::GrabImu, &imugb); 
  rclcpp::Subscription<sensor_msgs::msg::Imu>::SharedPtr sub_imu = 
    node->create_subscription<sensor_msgs::msg::Imu>("/imu", 1000, std::bind(&ImuGrabber::GrabImu, &imugb, _1));
  //rclcpp::Subscriber sub_img0 = n.subscribe("/camera/image_raw", 100, &ImageGrabber::GrabImage,&igb);
  rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr sub_img0 = 
    node->create_subscription<sensor_msgs::msg::Image>("/camera/image_raw", 100, std::bind(&ImageGrabber::GrabImage, &igb, _1));

  std::thread sync_thread(&ImageGrabber::SyncWithImu, &igb);

  rclcpp::spin(node);

  return 0;
}

void ImageGrabber::GrabImage(const sensor_msgs::msg::Image::ConstPtr img_msg)
{
  mBufMutex.lock();
  if (!img0Buf.empty())
    img0Buf.pop();
  img0Buf.push(img_msg);
  mBufMutex.unlock();
}

cv::Mat ImageGrabber::GetImage(const sensor_msgs::msg::Image::ConstPtr img_msg)
{
  // Copy the ros image message to cv::Mat.
  cv_bridge::CvImageConstPtr cv_ptr;
  try
  {
    cv_ptr = cv_bridge::toCvShare(img_msg, sensor_msgs::image_encodings::MONO8);
  }
  catch (cv_bridge::Exception& e)
  {
    //RCLCPP_ERROR(node->get_logger(), "cv_bridge exception: %s", e.what());
    std::cerr << "cv_bridge exception: " << e.what() << std::endl;
  }
  
  if(cv_ptr->image.type()==0)
  {
    return cv_ptr->image.clone();
  }
  else
  {
    std::cout << "Error type" << std::endl;
    return cv_ptr->image.clone();
  }
}

void ImageGrabber::SyncWithImu()
{
  while(1)
  {
    cv::Mat im;
    double tIm = 0;
    if (!img0Buf.empty()&&!mpImuGb->imuBuf.empty())
    {
      tIm = img0Buf.front()->header.stamp.sec;
      if(tIm>mpImuGb->imuBuf.back()->header.stamp.sec)
          continue;
      {
      this->mBufMutex.lock();
      im = GetImage(img0Buf.front());
      img0Buf.pop();
      this->mBufMutex.unlock();
      }

      vector<ORB_SLAM3::IMU::Point> vImuMeas;
      mpImuGb->mBufMutex.lock();
      if(!mpImuGb->imuBuf.empty())
      {
        // Load imu measurements from buffer
        vImuMeas.clear();
        while(!mpImuGb->imuBuf.empty() && mpImuGb->imuBuf.front()->header.stamp.sec<=tIm)
        {
          double t = mpImuGb->imuBuf.front()->header.stamp.sec;
          cv::Point3f acc(mpImuGb->imuBuf.front()->linear_acceleration.x, mpImuGb->imuBuf.front()->linear_acceleration.y, mpImuGb->imuBuf.front()->linear_acceleration.z);
          cv::Point3f gyr(mpImuGb->imuBuf.front()->angular_velocity.x, mpImuGb->imuBuf.front()->angular_velocity.y, mpImuGb->imuBuf.front()->angular_velocity.z);
          vImuMeas.push_back(ORB_SLAM3::IMU::Point(acc,gyr,t));
          mpImuGb->imuBuf.pop();
        }
      }
      mpImuGb->mBufMutex.unlock();
      if(mbClahe)
        mClahe->apply(im,im);

      std::chrono::steady_clock::time_point t1 = std::chrono::steady_clock::now();
      mpSLAM->TrackMonocular(im,tIm,vImuMeas);
      std::chrono::steady_clock::time_point t2 = std::chrono::steady_clock::now();
      double ttrack = std::chrono::duration_cast<std::chrono::duration<double> >(t2 - t1).count();
      std::cout << "ttrack: " << ttrack << std::endl;
    }

    std::chrono::milliseconds tSleep(1);
    std::this_thread::sleep_for(tSleep);
  }
}

void ImuGrabber::GrabImu(const sensor_msgs::msg::Imu::ConstPtr imu_msg)
{
  mBufMutex.lock();
  imuBuf.push(imu_msg);
  mBufMutex.unlock();
  return;
}