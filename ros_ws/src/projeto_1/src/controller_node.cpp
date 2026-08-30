#include <rclcpp/rclcpp.hpp>
#include <geometry_msgs/msg/pose.hpp>
#include <geometry_msgs/msg/twist_stamped.hpp>

#include <iostream>
#include <cmath>
#include <utility>
#include <chrono>

using namespace std;


double quaternionToYaw(double x, double y, double z, double w){
    double sin_yaw = 2.0 * (w * z + x * y);
    double cos_yaw = 1.0 - 2.0 * (y * y + z * z);

    return atan2(sin_yaw, cos_yaw);
}

double warpToPi(double angle){
    while (angle > M_PI){
        angle -= 2.0 * M_PI;
    }

    while (angle < -M_PI){
        angle += 2.0 * M_PI;
    }

    return angle;
}

double clampVelocity(double velocity, double max_velocity){
    if (velocity > max_velocity){
        return max_velocity;
    }

    if (velocity < -max_velocity){
        return -max_velocity;
    }

    return velocity;
}


pair<double, double> calculateVelocity(double x_target, double y_target, double yaw_target, double x_current, double y_current, double yaw_current){
    const double max_vx = 0.4;
    const double max_omega = 2.4;

    const double kv = 3.0;
    const double kw = 1.0;

    double dx = x_target - x_current;
    double dy = y_target - y_current;

    double distance = sqrt(dx * dx + dy * dy);

    if (distance < 0.1){
        double angle_error = warpToPi(yaw_current - yaw_target);

        if (abs(angle_error) < 0.1){
            return {0.0, 0.0};
        }

        double vx = 0.0;
        double omega = kw * warpToPi(yaw_target - yaw_current);

        vx = clampVelocity(vx, max_vx);
        omega = clampVelocity(omega, max_omega);

        return {vx, omega};
    }

    double theta_target = atan2(dy, dx);
    double dtheta = warpToPi(theta_target - yaw_current);

    double omega = kw * dtheta;
    double vx;

    if (abs(dtheta) > 0.1){
        vx = 0.0;
    }else {
        vx = kv * distance;
    }

    vx = clampVelocity(vx, max_vx);
    omega = clampVelocity(omega, max_omega);

    return {vx, omega};
}


class ControllerNode : public rclcpp::Node {
    private:
        double goal_x = 0.0;
        double goal_y = 0.0;
        double goal_yaw = 0.0;

        double current_x = 0.0;
        double current_y = 0.0;
        double current_yaw = 0.0;

        bool tem_goal = false;
        bool tem_robot_position = false;


        rclcpp::Subscription<geometry_msgs::msg::Pose>::SharedPtr goal_subscriber;
        rclcpp::Subscription<geometry_msgs::msg::Pose>::SharedPtr robot_position_subscriber;
        rclcpp::Publisher<geometry_msgs::msg::TwistStamped>::SharedPtr cmd_vel_publisher;
        rclcpp::TimerBase::SharedPtr timer;



        void updateController(){
            geometry_msgs::msg::TwistStamped cmd;
            cmd.header.stamp = this->get_clock()->now();

            cmd.twist.linear.x = 0.0;
            cmd.twist.linear.y = 0.0;
            cmd.twist.linear.z = 0.0;

            cmd.twist.angular.x = 0.0;
            cmd.twist.angular.y = 0.0;
            cmd.twist.angular.z = 0.0;

            if (!tem_goal || !tem_robot_position){
                cmd_vel_publisher->publish(cmd);
                return;
            }

            auto [vx, omega] = calculateVelocity(goal_x, goal_y, goal_yaw, current_x, current_y, current_yaw);

            cmd.twist.linear.x = vx;
            cmd.twist.angular.z = omega;

            cmd_vel_publisher->publish(cmd);
        }

    public:
        ControllerNode()
          : Node("controller_node") {

            goal_subscriber = this->create_subscription<geometry_msgs::msg::Pose>(
                "/goal",
                10,
                [this](const geometry_msgs::msg::Pose::SharedPtr msg){
                    goal_x = msg->position.x;
                    goal_y = msg->position.y;
                    goal_yaw = quaternionToYaw(msg->orientation.x, msg->orientation.y, msg->orientation.z, msg->orientation.w);
                    
                    tem_goal = true;
                }
            );


            robot_position_subscriber = this->create_subscription<geometry_msgs::msg::Pose>(
                "/robot_position",
                10,
                [this](const geometry_msgs::msg::Pose::SharedPtr msg){
                    current_x = msg->position.x;
                    current_y = msg->position.y;
                    current_yaw = quaternionToYaw(msg->orientation.x, msg->orientation.y, msg->orientation.z, msg->orientation.w);
                    
                    tem_robot_position = true;
                }
            );


            cmd_vel_publisher = this->create_publisher<geometry_msgs::msg::TwistStamped>(
                "/cmd_vel",
                10
            );



            timer = this->create_wall_timer(
                chrono::milliseconds(100),
                [this](){updateController();}
            );


        }


};



int main(int argc, char * argv[]){
    
    rclcpp::init(argc, argv);

    auto node = std::make_shared<ControllerNode>();

    rclcpp::spin(node);

    rclcpp::shutdown();

    return 0;
}