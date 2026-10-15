#include "rclcpp/rclcpp.hpp"
#include "custom_interfaces/srv/is_even.hpp"

#include <chrono>
#include <cstdlib>
#include <memory>

#include <sstream>

using namespace std::chrono_literals;

using IsEven = custom_interfaces::srv::IsEven;

int main(int argc, char* argv[])
{
    rclcpp::init(argc, argv);

    if (argc != 2){
        
        RCLCPP_ERROR(
            rclcpp::get_logger("rclcpp"),
            "Uso ros2 run cpp_nodes "
            "is_even_client_node <number>"
        );

        return 1;
    }

    auto node = rclcpp:: Node::make_shared("is_even_client");

    auto client = node->create_client<IsEven>("is_even");

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

    auto request = std::make_shared<IsEven::Request>();

    request->number = std::atoi(argv[1]);

    auto future = client->async_send_request(request);

    auto status = 
        rclcpp::spin_until_future_complete(
            node,
            future
        );

    if (status == rclcpp::FutureReturnCode::SUCCESS){
        
        auto response = future.get();

        std::stringstream ss;
        ss << std::boolalpha << response->is_even;

        RCLCPP_INFO(
            node->get_logger(),
            "O numero eh par? %s",
            ss.str().c_str()
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