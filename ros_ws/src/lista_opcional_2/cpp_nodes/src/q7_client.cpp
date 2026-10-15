#include "rclcpp/rclcpp.hpp"
#include "custom_interfaces/srv/array_statistics.hpp"

#include <chrono>
#include <cstdlib>
#include <memory>
#include <vector>


using namespace std::chrono_literals;

using ArrayStatistics = custom_interfaces::srv::ArrayStatistics;

int main(int argc, char* argv[])
{
    rclcpp::init(argc, argv);

    if (argc < 2) {

        RCLCPP_ERROR(
            rclcpp::get_logger("rclcpp"),
            "Uso: ros2 run cpp_nodes "
            "array_statistics_client_node <valor1> <valor2> ..."
        );

        rclcpp::shutdown();
        return 1;
    }

    auto node = rclcpp::Node::make_shared("array_statistics_client");

    auto client = node->create_client<ArrayStatistics>("array_statistics");

    while (!client->wait_for_service(1s)){

        if (!rclcpp::ok()){
            
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

    auto request = std::make_shared<ArrayStatistics::Request>();

    for (int i = 1; i < argc; i++) {
        request->values.push_back(atof(argv[i]));
    }

    auto future = client->async_send_request(request);

    auto status = 
        rclcpp::spin_until_future_complete(
            node,
            future
        );

    if (status == rclcpp::FutureReturnCode::SUCCESS){
        
        auto response = future.get();

        RCLCPP_INFO(
            node->get_logger(),
            "Valor mínimo: %f",
            response->minimum
        );

        RCLCPP_INFO(
            node->get_logger(),
            "Valor máximo: %f",
            response->maximum
        );

        RCLCPP_INFO(
            node->get_logger(),
            "Média: %f",
            response->average
        );

    } else {

        RCLCPP_ERROR(
            node->get_logger(),
            "Falha ao chamar o serviço."
        );
    }

    rclcpp::shutdown();

    return 0;
}