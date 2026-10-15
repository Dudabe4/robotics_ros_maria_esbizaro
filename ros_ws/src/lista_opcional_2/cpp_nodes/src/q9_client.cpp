#include "rclcpp/rclcpp.hpp"
#include "custom_interfaces/srv/closest_point.hpp"

#include "custom_interfaces/msg/point2_d.hpp"

#include <chrono>
#include <cstdlib>
#include <memory>
#include <vector>


using namespace std::chrono_literals;

using ClosestPoint = custom_interfaces::srv::ClosestPoint;
using Point2D = custom_interfaces::msg::Point2D;

int main(int argc, char* argv[])
{
    rclcpp::init(argc, argv);

    if (argc < 3 || (argc - 3) % 2 != 0) {
        RCLCPP_ERROR(
            rclcpp::get_logger("rclcpp"),
            "Uso: ros2 run cpp_nodes q9_client x_ref y_ref x1 y1 x2 y2 ..."
        );

        rclcpp::shutdown();
        return 1;
    }

    auto node = rclcpp::Node::make_shared("closest_point_client");

    auto client = node->create_client<ClosestPoint>("closest_point");

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

    auto request = std::make_shared<ClosestPoint::Request>();

    request->reference.x = std::atof(argv[1]);
    request->reference.y = std::atof(argv[2]);

    int cont = 3;

    while (cont < argc) {
        Point2D ponto;

        ponto.x = std::atof(argv[cont]);
        ponto.y = std::atof(argv[cont + 1]);

        request->candidates.push_back(ponto);

        cont += 2;
    }

    auto future = client->async_send_request(request);

    auto status = 
        rclcpp::spin_until_future_complete(
            node,
            future
        );

    if (status == rclcpp::FutureReturnCode::SUCCESS){
        
        auto response = future.get();

        if (response->index == -1) {
            RCLCPP_INFO(node->get_logger(), "Nenhum candidato foi recebido.");
        }
        else {
            RCLCPP_INFO(node->get_logger(), "Ponto mais proximo:");
            RCLCPP_INFO(node->get_logger(), "x = %.2f", response->closest.x);
            RCLCPP_INFO(node->get_logger(), "y = %.2f", response->closest.y);
            RCLCPP_INFO(node->get_logger(), "Distancia = %.4f", response->distance);
            RCLCPP_INFO(node->get_logger(), "Indice = %d", response->index);
        }

    } else {

        RCLCPP_ERROR(
            node->get_logger(),
            "Falha ao chamar o serviço."
        );
    }

    rclcpp::shutdown();

    return 0;
}