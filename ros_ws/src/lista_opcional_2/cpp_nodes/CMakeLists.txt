cmake_minimum_required(VERSION 3.8)
project(cpp_nodes)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(std_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(rclcpp REQUIRED)
find_package(custom_interfaces REQUIRED)

add_executable(q1_server
  src/q1_server.cpp
)
ament_target_dependencies(
  q1_server
  rclcpp
  custom_interfaces
)

add_executable(q1_client
  src/q1_client.cpp
)
ament_target_dependencies(
  q1_client
  rclcpp
  custom_interfaces
)

add_executable(q2_server
  src/q2_server.cpp
)
ament_target_dependencies(
  q2_server
  rclcpp
  custom_interfaces
)

add_executable(q2_client
  src/q2_client.cpp
)
ament_target_dependencies(
  q2_client
  rclcpp
  custom_interfaces
)

add_executable(q3_server
  src/q3_server.cpp
)
ament_target_dependencies(
  q3_server
  rclcpp
  custom_interfaces
)

add_executable(q3_client
  src/q3_client.cpp
)
ament_target_dependencies(
  q3_client
  rclcpp
  custom_interfaces
)

add_executable(q4_server
  src/q4_server.cpp
)
ament_target_dependencies(
  q4_server
  rclcpp
  custom_interfaces
)

add_executable(q4_client
  src/q4_client.cpp
)
ament_target_dependencies(
  q4_client
  rclcpp
  custom_interfaces
)

add_executable(q5_server
  src/q5_server.cpp
)
ament_target_dependencies(
  q5_server
  rclcpp
  custom_interfaces
)

add_executable(q5_client
  src/q5_client.cpp
)
ament_target_dependencies(
  q5_client
  rclcpp
  custom_interfaces
)

add_executable(q6_server
  src/q6_server.cpp
)
ament_target_dependencies(
  q6_server
  rclcpp
  custom_interfaces
)

add_executable(q6_client
  src/q6_client.cpp
)
ament_target_dependencies(
  q6_client
  rclcpp
  custom_interfaces
)

add_executable(q7_server
  src/q7_server.cpp
)
ament_target_dependencies(
  q7_server
  rclcpp
  custom_interfaces
)

add_executable(q7_client
  src/q7_client.cpp
)
ament_target_dependencies(
  q7_client
  rclcpp
  custom_interfaces
)

add_executable(q8_server
  src/q8_server.cpp
)
ament_target_dependencies(
  q8_server
  rclcpp
  custom_interfaces
)

add_executable(q8_client
  src/q8_client.cpp
)
ament_target_dependencies(
  q8_client
  rclcpp
  custom_interfaces
)

add_executable(q9_server
  src/q9_server.cpp
)
ament_target_dependencies(
  q9_server
  rclcpp
  custom_interfaces
)

add_executable(q9_client
  src/q9_client.cpp
)
ament_target_dependencies(
  q9_client
  rclcpp
  custom_interfaces
)

add_executable(q10_server
  src/q10_server.cpp
)
ament_target_dependencies(
  q10_server
  rclcpp
  custom_interfaces
)

add_executable(q10_client
  src/q10_client.cpp
)
ament_target_dependencies(
  q10_client
  rclcpp
  custom_interfaces
)

install(
  TARGETS
    q1_server
    q1_client
    q2_server
    q2_client
    q3_server
    q3_client
    q4_server
    q4_client
    q5_server
    q5_client
    q6_server
    q6_client
    q7_server
    q7_client
    q8_server
    q8_client
    q9_server
    q9_client
    q10_server
    q10_client
  DESTINATION
    lib/${PROJECT_NAME}
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
