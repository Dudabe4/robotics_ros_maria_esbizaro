#include "rclcpp/rclcpp.hpp"
#include "custom_interfaces/srv/closest_point.hpp"

#include "custom_interfaces/msg/point2_d.hpp"

#include <cmath>

// Trate o caso de candidates vazio devolvendo index igual a -1 .
using ClosestPoint = custom_interfaces::srv::ClosestPoint;

class ClosestPointServer : public rclcpp::Node 
{
public:

    ClosestPointServer() : Node("closest_point_server")
    {
        service_ = this->create_service<ClosestPoint>(
            "closest_point",
            std::bind(
                &ClosestPointServer::callback,
                this,
                std::placeholders::_1,
                std::placeholders::_2
            )
        );

        RCLCPP_INFO(
            this->get_logger(),
            "Servidor de distância iniciado."
        );
    }

private:

    void callback(
        const std::shared_ptr<ClosestPoint::Request> request,
        std::shared_ptr<ClosestPoint::Response> response
    ) {

        auto reference = request->reference;
        auto candidates = request->candidates;

        int tam = candidates.size();

        if (tam == 0) {
            response->closest.x = 0.0;
            response->closest.y = 0.0;
            response->distance = 0.0;
            response->index = -1;
        }
        else {

            float menor_distancia = INFINITY;
            int indice = -1;

            int cont = 0;

            while (cont < tam) {
                
                float dx = candidates[cont].x - reference.x;
                float dy = candidates[cont].y - reference.y;

                float distancia = std::sqrt(
                    (dx * dx) + (dy * dy)
                );

                if (distancia < menor_distancia) {
                    menor_distancia = distancia;
                    indice = cont;
                }

                cont++;
            }


            response->closest = candidates[indice];
            response->distance = menor_distancia;
            response->index = indice;
        }

        RCLCPP_INFO(
            this->get_logger(),
            "Closest = (%.2f, %.2f) | Distance = %.2f | Index = %d",
            response->closest.x,
            response->closest.y,
            response->distance,
            response->index
        );
        
    }

    rclcpp::Service<ClosestPoint>::SharedPtr service_;
};

int main(int argc, char* argv[]){

    rclcpp::init(argc, argv);

    auto node = std::make_shared<ClosestPointServer>();

    rclcpp::spin(node);

    rclcpp::shutdown();

    return 0;
}