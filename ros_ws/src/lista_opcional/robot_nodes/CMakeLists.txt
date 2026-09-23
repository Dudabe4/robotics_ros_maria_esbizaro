cmake_minimum_required(VERSION 3.8)
project(robot_nodes)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()


# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(robot_interfaces REQUIRED)

# --- Nó em C++ ---
add_executable(velocity_history_subscriber src/velocity_history_subscriber.cpp)
add_executable(laser_data_subscriber src/laser_data_subscriber.cpp)
add_executable(robot_pose_subscriber src/robot_pose_subscriber.cpp)
add_executable(path_subscriber src/path_subscriber.cpp)

ament_target_dependencies(velocity_history_subscriber
  rclcpp
  geometry_msgs
  robot_interfaces
)

ament_target_dependencies(laser_data_subscriber
  rclcpp
  geometry_msgs
  robot_interfaces
)

ament_target_dependencies(robot_pose_subscriber
  rclcpp
  geometry_msgs
  robot_interfaces
)

ament_target_dependencies(path_subscriber
  rclcpp
  geometry_msgs
  robot_interfaces
)

install(TARGETS
  velocity_history_subscriber
  laser_data_subscriber
  robot_pose_subscriber
  path_subscriber
  DESTINATION lib/${PROJECT_NAME}
)

# --- Nó em Python ---
# Em um pacote ament_cmake, scripts Python sao instalados com install(PROGRAMS),
# que e o equivalente ao console_scripts do setup.py
install(PROGRAMS
  scripts/robot_state_publisher.py
  scripts/sensor_info_publisher.py
  scripts/velocity_history_publisher.py
  scripts/laser_data_publisher.py
  scripts/robot_pose_publisher.py
  scripts/path_publisher.py
  DESTINATION lib/${PROJECT_NAME}
)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # comment the line when a copyright and license is added to all source files
  set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # comment the line when this package is in a git repo and when
  # a copyright and license is added to all source files
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()
