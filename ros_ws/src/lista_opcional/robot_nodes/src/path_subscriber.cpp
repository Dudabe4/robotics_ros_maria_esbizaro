#include <memory>

#include "rclcpp/rclcpp.hpp"

#include "robot_interfaces/msg/path.hpp"

class PathSubscriber : public rclcpp::Node
{
public:
    PathSubscriber() : Node("path_subscriber")
    {
        subscription_ = this->create_subscription<
            robot_interfaces::msg::Path>(
            "/path",
            10,
            std::bind(
                &PathSubscriber::callback,
                this,
                std::placeholders::_1
            )
        );
    }

private:
    void callback(
        const robot_interfaces::msg::Path::SharedPtr msg)
    {
        RCLCPP_INFO(
            this->get_logger(),
            "Trajetoria recebida com %zu poses:",
            msg->poses.size()
        );

        for (size_t i = 0; i < msg->poses.size(); i++)
        {
            double x = msg->poses[i].position.x;
            double y = msg->poses[i].position.y;
            double z = msg->poses[i].position.z;

            double ox = msg->poses[i].orientation.x;
            double oy = msg->poses[i].orientation.y;
            double oz = msg->poses[i].orientation.z;
            double ow = msg->poses[i].orientation.w;

            RCLCPP_INFO(
                this->get_logger(),
                "Pose %zu:",
                i + 1
            );

            RCLCPP_INFO(
                this->get_logger(),
                "  Posicao: x=%.2f, y=%.2f, z=%.2f",
                x, y, z
            );

            RCLCPP_INFO(
                this->get_logger(),
                "  Orientacao: x=%.2f, y=%.2f, z=%.2f, w=%.2f",
                ox, oy, oz, ow
            );
        }
    }


    rclcpp::Subscription<
        robot_interfaces::msg::Path>::SharedPtr subscription_;
};

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);

    auto node = std::make_shared<PathSubscriber>();

    rclcpp::spin(node);

    rclcpp::shutdown();

    return 0;
}