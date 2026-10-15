#include "rclcpp/rclcpp.hpp"
#include "custom_interfaces/srv/convert_temperature.hpp"

#include <chrono>
#include <cstdlib>
#include <memory>


using namespace std::chrono_literals;

using ConvertTemperature = custom_interfaces::srv::ConvertTemperature;

int main(int argc, char* argv[])
{
    rclcpp::init(argc, argv);

    if (argc != 2){
        
        RCLCPP_ERROR(
            rclcpp::get_logger("rclcpp"),
            "Uso ros2 run cpp_nodes "
            "convert_temperature_client_node <celsius>"
        );

        return 1;
    }

    auto node = rclcpp:: Node::make_shared("convert_temperature_client");

    auto client = node->create_client<ConvertTemperature>("convert_temperature");

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

    auto request = std::make_shared<ConvertTemperature::Request>();

    request->celsius = std::atof(argv[1]);

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
            "O numero em fahrenheit é? %f F",
            response->fahrenheit
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