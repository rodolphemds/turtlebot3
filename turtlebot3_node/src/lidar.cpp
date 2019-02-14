/*******************************************************************************
* Copyright 2017 ROBOTIS CO., LTD.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*******************************************************************************/

/* Author: Darby Lim */

#include "lidar.h"

using namespace turtlebot3;

void Lidar::makeFullRange(const sensor_msgs::msg::LaserScan::ConstSharedPtr& msg)
{
  std::lock_guard<std::mutex> lock(laser_scan_msg_mutex_);
  laser_scan_msg_.header.frame_id = msg->header.frame_id;
  laser_scan_msg_.header.stamp = msg->header.stamp;

  laser_scan_msg_.angle_min = msg->angle_min;
  laser_scan_msg_.angle_max = msg->angle_max;
  laser_scan_msg_.angle_increment = msg->angle_increment;
  laser_scan_msg_.time_increment = msg->time_increment;
  laser_scan_msg_.scan_time = msg->scan_time;
  laser_scan_msg_.range_min = msg->range_min;
  laser_scan_msg_.range_max = msg->range_max; 
  laser_scan_msg_.ranges.resize(360);

  for (uint16_t i=0, j=0; i<180; j=j+2, i++)
  {
    laser_scan_msg_.ranges[j] = msg->ranges[i];
    laser_scan_msg_.ranges[j+1] = msg->ranges[i];
  }
}

sensor_msgs::msg::LaserScan Lidar::getLaserScan(const rclcpp::Time now)
{
  std::lock_guard<std::mutex> lock(laser_scan_msg_mutex_);
  laser_scan_msg_.header.stamp = now;
  return laser_scan_msg_;
}


#if 0
rclcpp::Node::SharedPtr node = nullptr;
auto laser_scan = std::make_shared<sensor_msgs::msg::LaserScan>();

void scanHalfMsgCallback(const sensor_msgs::msg::LaserScan::SharedPtr msg)
{
  laser_scan->header.frame_id = msg->header.frame_id;
  laser_scan->header.stamp = msg->header.stamp;

  laser_scan->angle_min = msg->angle_min;
  laser_scan->angle_max = msg->angle_max;
  laser_scan->angle_increment = msg->angle_increment;
  laser_scan->time_increment = msg->time_increment;
  laser_scan->scan_time = msg->scan_time;
  laser_scan->range_min = msg->range_min;
  laser_scan->range_max = msg->range_max;
  laser_scan->ranges.resize(360);
  laser_scan->intensities.resize(360);

  uint16_t j = 0;
  for (uint16_t i = 0; i < 180; i++)
  {
    laser_scan->ranges[j] = msg->ranges[i];
    laser_scan->ranges[j+1] = msg->ranges[i];

    laser_scan->intensities[j] = 0.0f;
    laser_scan->intensities[j+1] = 0.0f;

    j += 2;
  }
}

int main(int argc, char *argv[])
{
  rclcpp::init(argc, argv);

  node = rclcpp::Node::make_shared("scan_publisher");

  RCLCPP_INFO(node->get_logger(), "Init scan publisher");

  rmw_qos_profile_t scan_qos_profile = rmw_qos_profile_sensor_data;
  scan_qos_profile.history = RMW_QOS_POLICY_HISTORY_KEEP_LAST;
  scan_qos_profile.depth = 1;
  scan_qos_profile.reliability = RMW_QOS_POLICY_RELIABILITY_RELIABLE;
  scan_qos_profile.durability = RMW_QOS_POLICY_DURABILITY_VOLATILE;

  auto scan_half_sub = node->create_subscription<sensor_msgs::msg::LaserScan>("scan_half", scanHalfMsgCallback, scan_qos_profile);
  auto scan_pub = node->create_publisher<sensor_msgs::msg::LaserScan>("scan");

  rclcpp::WallRate loop_rate(1ms);

  while (rclcpp::ok())
  {
    scan_pub->publish(laser_scan);

    rclcpp::spin_some(node);
    loop_rate.sleep();
  }
  rclcpp::shutdown();

  scan_half_sub = nullptr;
  scan_pub = nullptr;
  node = nullptr;
  return 0;
}
#endif