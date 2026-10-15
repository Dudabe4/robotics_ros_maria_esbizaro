#include "rclcpp/rclcpp.hpp"
#include "custom_interfaces/srv/linear_regression.hpp"

#include <chrono>
#include <cstdlib>
#include <memory>
#include <vector>


using namespace std::chrono_literals;

using LinearRegression = custom_interfaces::srv::LinearRegression;

int main(int argc, char* argv[])
{
    rclcpp::init(argc, argv);

    if (argc < 5 || (argc - 1) % 2 != 0) {

        RCLCPP_ERROR(
            rclcpp::get_logger("rclcpp"),
            "Uso: ros2 run cpp_nodes linear_regression_client_node x1 x2 ... y1 y2 ..."
        );

        rclcpp::shutdown();
        return 1;
    }

    auto node = rclcpp::Node::make_shared("linear_regression_client");

    auto client = node->create_client<LinearRegression>("linear_regression");

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

    auto request = std::make_shared<LinearRegression::Request>();

    int tam = (argc - 1) / 2;

    for (int i = 1; i <= tam; i++) {
        request->x.push_back(std::atof(argv[i]));
    }

    for (int i = tam + 1; i < argc; i++) {
        request->y.push_back(std::atof(argv[i]));
    }

    auto future = client->async_send_request(request);

    auto status = 
        rclcpp::spin_until_future_complete(
            node,
            future
        );

    if (status == rclcpp::FutureReturnCode::SUCCESS){
        
        auto response = future.get();

        RCLCPP_INFO(node->get_logger(), "Resultado da regressao linear:");
        RCLCPP_INFO(node->get_logger(), "Slope = %f", response->slope);
        RCLCPP_INFO(node->get_logger(), "Intercept = %f", response->intercept);
        RCLCPP_INFO(node->get_logger(), "R_squared = %f", response->r_squared);

    } else {

        RCLCPP_ERROR(
            node->get_logger(),
            "Falha ao chamar o serviço."
        );
    }

    rclcpp::shutdown();

    return 0;
}