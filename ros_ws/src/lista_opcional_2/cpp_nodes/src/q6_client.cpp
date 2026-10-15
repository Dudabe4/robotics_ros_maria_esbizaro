#include "rclcpp/rclcpp.hpp"
#include "custom_interfaces/srv/dot_product.hpp"

#include "geometry_msgs/msg/vector3.hpp"

#include <chrono>
#include <cstdlib>
#include <memory>


using namespace std::chrono_literals;

using DotProduct = custom_interfaces::srv::DotProduct;

int main(int argc, char* argv[])
{
    rclcpp::init(argc, argv);

    if (argc != 7){
        
        RCLCPP_ERROR(
            rclcpp::get_logger("rclcpp"),
            "Uso ros2 run cpp_nodes "
            "dot_product_client_node <vector_a> <vector_b>"
        );

        return 1;
    }

    auto node = rclcpp:: Node::make_shared("dot_product_client");

    auto client = node->create_client<DotProduct>("dot_product");

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

    auto request = std::make_shared<DotProduct::Request>();

    float x_a = atof(argv[1]);
    float y_a = atof(argv[2]);
    float z_a = atof(argv[3]);

    float x_b = atof(argv[4]);
    float y_b = atof(argv[5]);
    float z_b = atof(argv[6]);

    auto vet_a = geometry_msgs::msg::Vector3();
    vet_a.x = x_a;
    vet_a.y = y_a;
    vet_a.z = z_a;

    auto vet_b = geometry_msgs::msg::Vector3();
    vet_b.x = x_b;
    vet_b.y = y_b;
    vet_b.z = z_b;

    request->vector_a = vet_a;
    request->vector_b = vet_b;

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
            "O produto escalar dos vetores é %f",
            response->result
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