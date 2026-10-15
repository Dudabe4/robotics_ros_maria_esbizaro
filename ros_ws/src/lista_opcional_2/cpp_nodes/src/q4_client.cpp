#include "rclcpp/rclcpp.hpp"
#include "custom_interfaces/srv/get_circle_area.hpp"

#include <chrono>
#include <cstdlib>
#include <memory>


using namespace std::chrono_literals;

using GetCircleArea = custom_interfaces::srv::GetCircleArea;

int main(int argc, char* argv[])
{
    rclcpp::init(argc, argv);

    if (argc != 2){
        
        RCLCPP_ERROR(
            rclcpp::get_logger("rclcpp"),
            "Uso ros2 run cpp_nodes "
            "get_circle_area_client_node <radius>"
        );

        return 1;
    }

    auto node = rclcpp:: Node::make_shared("get_circle_area_client");

    auto client = node->create_client<GetCircleArea>("get_circle_area");

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

    auto request = std::make_shared<GetCircleArea::Request>();

    request->radius = std::atof(argv[1]);

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
            "Sua área é %lf",
            response->area
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