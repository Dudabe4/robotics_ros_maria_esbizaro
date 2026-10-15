#include "rclcpp/rclcpp.hpp"
#include "custom_interfaces/srv/reverse_string.hpp"

#include <chrono>
#include <cstdlib>
#include <memory>

#include <string>


using namespace std::chrono_literals;

using ReverseString = custom_interfaces::srv::ReverseString;

int main(int argc, char* argv[])
{
    rclcpp::init(argc, argv);

    if (argc != 2){
        
        RCLCPP_ERROR(
            rclcpp::get_logger("rclcpp"),
            "Uso ros2 run cpp_nodes "
            "reverse_string_client_node <text>"
        );

        return 1;
    }

    auto node = rclcpp:: Node::make_shared("reverse_string_client");

    auto client = node->create_client<ReverseString>("reverse_string");

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

    auto request = std::make_shared<ReverseString::Request>();

    request->text = (argv[1]);

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
            "O contrário da palavra é %s",
            (response->reversed_text).c_str()
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