#include <memory>

#include "rclcpp/rclcpp.hpp"

#include "robot_interfaces/msg/robot_pose.hpp"

class RobotPoseSubscriber : public rclcpp::Node
{
public:
    RobotPoseSubscriber() : Node("robot_pose_subscriber")
    {
        subscription_ = this->create_subscription<
            robot_interfaces::msg::RobotPose>(
            "/robot_pose",
            10,
            std::bind(
                &RobotPoseSubscriber::callback,
                this,
                std::placeholders::_1
            )
        );
    }

private:
    void callback(
        const robot_interfaces::msg::RobotPose::SharedPtr msg)
    {
        RCLCPP_INFO(
            this->get_logger(),
            "Pose:"
        );

        RCLCPP_INFO(
            this->get_logger(),
            "  Posicao: x=%.2f, y=%.2f, z=%.2f",
            msg->pose.position.x,
            msg->pose.position.y,
            msg->pose.position.z
        );

        double yaw = std::atan2(
            2.0 * (
                msg->pose.orientation.w * msg->pose.orientation.z +
                msg->pose.orientation.x * msg->pose.orientation.y
            ),
            1.0 - 2.0 * (
                msg->pose.orientation.y * msg->pose.orientation.y +
                msg->pose.orientation.z * msg->pose.orientation.z
            )
        );

        RCLCPP_INFO(
            this->get_logger(),
            "  Yaw: %.2f rad",
            yaw
        );

        RCLCPP_INFO(
            this->get_logger(),
            "Velocidade:"
        );

        RCLCPP_INFO(
            this->get_logger(),
            "  Linear: x=%.2f, y=%.2f, z=%.2f",
            msg->velocity.linear.x,
            msg->velocity.linear.y,
            msg->velocity.linear.z
        );

        RCLCPP_INFO(
            this->get_logger(),
            "  Angular: x=%.2f, y=%.2f, z=%.2f",
            msg->velocity.angular.x,
            msg->velocity.angular.y,
            msg->velocity.angular.z
        );

        double dt = 10.0;

        double x = msg->pose.position.x;
        double y = msg->pose.position.y;

        double velocidade = msg->velocity.linear.x;

        double proximo_x = x + velocidade * std::cos(yaw) * dt;
        double proximo_y = y + velocidade * std::sin(yaw) * dt;

        double velocidade_angular = msg->velocity.angular.z;

        double novo_yaw = yaw + velocidade_angular * dt;

        RCLCPP_INFO(
            this->get_logger(),
            "Proxima posicao em %.1lf s: x=%.2lf, y=%.2lf, yaw=%.2lf",
            dt,
            proximo_x,
            proximo_y,
            novo_yaw
        );

    }

    rclcpp::Subscription<
        robot_interfaces::msg::RobotPose>::SharedPtr subscription_;
};

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);

    auto node = std::make_shared<RobotPoseSubscriber>();

    rclcpp::spin(node);

    rclcpp::shutdown();

    return 0;
}