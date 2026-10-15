#include "rclcpp/rclcpp.hpp"
#include "custom_interfaces/srv/subtract_two_ints.hpp"

#include <chrono>
#include <cstdlib>
#include <memory>

using namespace std::chrono_literals;

using SubtractTwoInts = custom_interfaces::srv::SubtractTwoInts;

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);

    if (argc != 3)
    {
        RCLCPP_ERROR(
            rclcpp::get_logger("rclcpp"),
            "Uso: ros2 run cpp_nodes "
            "subtract_ints_client_node <a> <b>"
        );

        return 1;
    }

    auto node = rclcpp::Node::make_shared("subtract_ints_client");

    auto client =
        node->create_client<SubtractTwoInts>("subtract_two_ints");

    while (!client->wait_for_service(1s))
    {
        if (!rclcpp::ok())
        {
            RCLCPP_ERROR(
                rclcpp::get_logger("rclcpp"),
                "Interrupção durante espera pelo serviço."
            );

            return 1;
        }

        RCLCPP_INFO(
            rclcpp::get_logger("rclcpp"),
            "Esperando pelo serviço..."
        );
    }

    auto request = std::make_shared<SubtractTwoInts::Request>();

    request->a = std::atoi(argv[1]);
    request->b = std::atoi(argv[2]);

    auto future = client->async_send_request(request);

    auto status =
        rclcpp::spin_until_future_complete(
            node,
            future
        );

    if (status == rclcpp::FutureReturnCode::SUCCESS)
    {
        auto response = future.get();

        RCLCPP_INFO(
            node->get_logger(),
            "Resultado: %d",
            response->difference
        );
    }
    else
    {
        RCLCPP_ERROR(
            node->get_logger(),
            "Falha ao chamar o serviço."
        );
    }

    rclcpp::shutdown();

    return 0;
}
