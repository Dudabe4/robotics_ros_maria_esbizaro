#include "rclcpp/rclcpp.hpp"
#include "custom_interfaces/srv/inverse_kinematics2_link.hpp"

#include <cmath>
#include <memory>
#include <algorithm>

using InverseKinematics2Link = custom_interfaces::srv::InverseKinematics2Link;

class InverseKinematicsServer : public rclcpp::Node 
{
public:

    InverseKinematicsServer() : Node("inverse_kinematics_2link_server")
    {
        service_ = this->create_service<InverseKinematics2Link>(
            "inverse_kinematics_2link",
            std::bind(
                &InverseKinematicsServer::callback,
                this,
                std::placeholders::_1,
                std::placeholders::_2
            )
        );

        RCLCPP_INFO(
            this->get_logger(),
            "Servidor de cinemática inversa iniciado."
        );
    }

private:

    void callback(
        const std::shared_ptr<InverseKinematics2Link::Request> request,
        std::shared_ptr<InverseKinematics2Link::Response> response
    ) {

        double x = request->target_x;
        double y = request->target_y;
        double l1 = request->link1_length;
        double l2 = request->link2_length;

        if (l1 <= 0.0 || l2 <= 0.0) {
            response->theta1 = 0.0;
            response->theta2 = 0.0;
            response->reachable = false;

            RCLCPP_WARN(
                this->get_logger(),
                "Comprimentos dos elos invalidos."
            );
            return;
        }

        double r = std::sqrt(x * x + y * y);

        if (r > l1 + l2 || r < std::abs(l1 - l2)) {
            response->theta1 = 0.0;
            response->theta2 = 0.0;
            response->reachable = false;

            RCLCPP_INFO(
                this->get_logger(),
                "Alvo fora do alcance do braco."
            );
            return;
        }

        double cos_theta2 = (r * r - l1 * l1 - l2 * l2) / (2.0 * l1 * l2);

        cos_theta2 = std::clamp(cos_theta2, -1.0, 1.0);

        double theta2 = std::acos(cos_theta2);

        double k1 = l1 + l2 * std::cos(theta2);
        double k2 = l2 * std::sin(theta2);

        double theta1 = std::atan2(y, x) - std::atan2(k2, k1);

        response->theta1 = theta1;
        response->theta2 = theta2;
        response->reachable = true;

        RCLCPP_INFO(
            this->get_logger(),
            "Alvo alcancavel: theta1 = %.4f rad, theta2 = %.4f rad",
            response->theta1,
            response->theta2
        );
        
    }

    rclcpp::Service<InverseKinematics2Link>::SharedPtr service_;
};

int main(int argc, char* argv[]){

    rclcpp::init(argc, argv);

    auto node = std::make_shared<InverseKinematicsServer>();

    rclcpp::spin(node);

    rclcpp::shutdown();

    return 0;
}