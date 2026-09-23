#include <memory>

#include "rclcpp/rclcpp.hpp"

#include "robot_interfaces/msg/laser_data.hpp"


class LaserDataSubscriber : public rclcpp::Node
{
public:
    LaserDataSubscriber() : Node("laser_data_subscriber")
    {
        subscription_ = this->create_subscription<
            robot_interfaces::msg::LaserData>(
            "/laser_data",
            10,
            std::bind(
                &LaserDataSubscriber::callback,
                this,
                std::placeholders::_1
            )
        );
    }

private:
    void callback(
        const robot_interfaces::msg::LaserData::SharedPtr msg)
    {
       if (msg->dists.empty())
        {
            RCLCPP_WARN(
                this->get_logger(),
                "Nenhuma medicao recebida"
            );
            return;
        }

        float menor_distancia = msg->dists[0];

        for (size_t i = 1; i < msg->dists.size(); i++)
        {
            if (msg->dists[i] < menor_distancia)
            {
               menor_distancia = msg->dists[i];
            }
        }

        RCLCPP_INFO(
            this->get_logger(),
            "Menor distancia: %.2f m",
            menor_distancia
        );
    }

    rclcpp::Subscription<
        robot_interfaces::msg::LaserData>::SharedPtr subscription_;
};

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);

    auto node = std::make_shared<LaserDataSubscriber>();

    rclcpp::spin(node);

    rclcpp::shutdown();

    return 0;
}