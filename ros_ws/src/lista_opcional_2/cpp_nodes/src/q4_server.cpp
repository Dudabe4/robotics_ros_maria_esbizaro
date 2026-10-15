#include "rclcpp/rclcpp.hpp"
#include "custom_interfaces/srv/get_circle_area.hpp"

#include <cmath>

using GetCircleArea = custom_interfaces::srv::GetCircleArea;

class GetCircleAreaServer : public rclcpp::Node 
{
public:

    GetCircleAreaServer() : Node("get_circle_area_server")
    {
        service_ = this->create_service<GetCircleArea>(
            "get_circle_area",
            std::bind(
                &GetCircleAreaServer::callback,
                this,
                std::placeholders::_1,
                std::placeholders::_2
            )
        );

        RCLCPP_INFO(
            this->get_logger(),
            "Servidor de cálculo de área iniciado."
        );
    }

private:

    void callback(
        const std::shared_ptr<GetCircleArea::Request> request,
        std::shared_ptr<GetCircleArea::Response> response
    ) {

        double raio = request->radius;

        if (raio >= 0){

            response->area = M_PI*pow(raio, 2);

            RCLCPP_INFO(
                this->get_logger(),
                "O círculo de raio %lf tem a área igual a %lf",
                request->radius,
                response->area
            );
        } else {
            response->area = -1.0;

            RCLCPP_INFO(
                this->get_logger(),
                "Área = %lf . O círculo de raio %lf não existe pelo raio ser negativo.",
                response->area,
                request->radius
            );
        }
    }

    rclcpp::Service<GetCircleArea>::SharedPtr service_;
};

int main(int argc, char* argv[]){

    rclcpp::init(argc, argv);

    auto node = std::make_shared<GetCircleAreaServer>();

    rclcpp::spin(node);

    rclcpp::shutdown();

    return 0;
}