#include "rclcpp/rclcpp.hpp"
#include "custom_interfaces/srv/inverse_kinematics2_link.hpp"

#include <chrono>
#include <cstdlib>
#include <memory>
#include <vector>


using namespace std::chrono_literals;

using InverseKinematics2Link = custom_interfaces::srv::InverseKinematics2Link;

int main(int argc, char* argv[])
{
    rclcpp::init(argc, argv);

    if (argc != 5) {
        RCLCPP_ERROR(
            rclcpp::get_logger("rclcpp"),
            "Uso: ros2 run cpp_nodes q10_client target_x target_y link1_length link2_length"
        );

        rclcpp::shutdown();
        return 1;
    }

    auto node = rclcpp::Node::make_shared("inverse_kinematics_2link_client");

    auto client = node->create_client<InverseKinematics2Link>("inverse_kinematics_2link");

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

    auto request =
        std::make_shared<InverseKinematics2Link::Request>();

    request->target_x = std::atof(argv[1]);
    request->target_y = std::atof(argv[2]);
    request->link1_length = std::atof(argv[3]);
    request->link2_length = std::atof(argv[4]);

    auto future = client->async_send_request(request);

    auto status = 
        rclcpp::spin_until_future_complete(
            node,
            future
        );

    if (status == rclcpp::FutureReturnCode::SUCCESS){
        
        auto response = future.get();

        if (response->reachable) {
            RCLCPP_INFO(node->get_logger(), "Alvo alcancavel!");
            RCLCPP_INFO(
                node->get_logger(),
                "theta1 = %.4f rad",
                response->theta1
            );
            RCLCPP_INFO(
                node->get_logger(),
                "theta2 = %.4f rad",
                response->theta2
            );
        } else {
            RCLCPP_INFO(
                node->get_logger(),
                "Alvo inalcançavel. Os angulos foram zerados."
            );
            RCLCPP_INFO(
                node->get_logger(),
                "theta1 = %.2f | theta2 = %.2f",
                response->theta1,
                response->theta2
            );
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