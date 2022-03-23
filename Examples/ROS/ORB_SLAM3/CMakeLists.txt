cmake_minimum_required(VERSION 3.5)
project(ORB_SLAM3)

# Default to C++14
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 14)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic -O3 -march=native)
endif()

LIST(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/../../../cmake_modules)

# find dependencies
find_package(ament_cmake REQUIRED)
# uncomment the following section in order to fill in
# further dependencies manually.
# find_package(<dependency> REQUIRED)
find_package(rclcpp REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(cv_bridge REQUIRED PATHS /home/shared/vision_opencv/build/cv_bridge)

find_package(OpenCV 4.4 PATHS /home/shared/opencv-4.5.5/build)
if(NOT OpenCV_FOUND)
	message(FATAL_ERROR "OpenCV > 4.4 not found.")
endif()

find_package(Eigen3 3.1.0 REQUIRED)
find_package(Pangolin REQUIRED)

add_executable(mono_inertial src/mono_inertial.cpp)
target_include_directories(mono_inertial PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
  ${PROJECT_SOURCE_DIR}/../../../
  ${PROJECT_SOURCE_DIR}/../../../include
  ${PROJECT_SOURCE_DIR}/../../../include/CameraModels
  ${PROJECT_SOURCE_DIR}/../../../Thirdparty/Sophus
  ${Pangolin_INCLUDE_DIRS}
  ${OpenCV_INCLUDE_DIRS}
)
target_link_libraries(mono_inertial 
  ${LIBS}
#  ${OpenCV_LIBS}
  /home/shared/opencv-4.5.5/build/lib
  ${EIGEN3_LIBS}
  ${Pangolin_LIBRARIES}
  ${PROJECT_SOURCE_DIR}/../../../Thirdparty/DBoW2/lib/libDBoW2.so
  ${PROJECT_SOURCE_DIR}/../../../Thirdparty/g2o/lib/libg2o.so
  ${PROJECT_SOURCE_DIR}/../../../lib/libORB_SLAM3.so
  -lboost_system
)
ament_target_dependencies(mono_inertial
  rclcpp
  sensor_msgs
#  cv_bridge
)

install(TARGETS mono_inertial
  DESTINATION lib/${PROJECT_NAME})

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # uncomment the line when a copyright and license is not present in all source files
  #set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # uncomment the line when this package is not in a git repo
  #set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()
