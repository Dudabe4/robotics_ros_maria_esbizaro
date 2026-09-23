#include <memory>

#include "rclcpp/rclcpp.hpp"

#include "robot_interfaces/msg/velocity_history.hpp"

class VelocityHistorySubscriber : public rclcpp::Node
{
public:
    VelocityHistorySubscriber() : Node("velocity_history_subscriber")
    {
        subscription_ = this->create_subscription<
            robot_interfaces::msg::VelocityHistory>(
            "/velocity_history",
            10,
            std::bind(
                &VelocityHistorySubscriber::callback,
                this,
                std::placeholders::_1
            )
        );
    }

private:
    void callback(
        const robot_interfaces::msg::VelocityHistory::SharedPtr msg)
    {
        RCLCPP_INFO(
            this->get_logger(),
            "Quantidade de amostras: %u",
            msg->sample_count
        );

        for (size_t i = 0; i < msg->linear_velocities.size(); i++)
        {
            RCLCPP_INFO(
                this->get_logger(),
                "Amostra %zu | Linear: %.2f | Angular: %.2f",
                i,
                msg->linear_velocities[i],
                msg->angular_velocities[i]
            );
        }
    }

    rclcpp::Subscription<
        robot_interfaces::msg::VelocityHistory>::SharedPtr subscription_;
};

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);

    auto node = std::make_shared<VelocityHistorySubscriber>();

    rclcpp::spin(node);

    rclcpp::shutdown();

    return 0;
}