#include "rclcpp/rclcpp.hpp"
#include "custom_interfaces/srv/array_statistics.hpp"

#include "geometry_msgs/msg/vector3.hpp"

#include <cmath>


using ArrayStatistics = custom_interfaces::srv::ArrayStatistics;

class ArrayStatisticsServer : public rclcpp::Node 
{
public:

    ArrayStatisticsServer() : Node("array_statistics_server")
    {
        service_ = this->create_service<ArrayStatistics>(
            "array_statistics",
            std::bind(
                &ArrayStatisticsServer::callback,
                this,
                std::placeholders::_1,
                std::placeholders::_2
            )
        );

        RCLCPP_INFO(
            this->get_logger(),
            "Servidor de estatísticas iniciado."
        );
    }

private:

    void callback(
        const std::shared_ptr<ArrayStatistics::Request> request,
        std::shared_ptr<ArrayStatistics::Response> response
    ) {


        auto valores = request->values;
        int tam = valores.size();
    

        if (tam == 0){
            response->minimum = 0;
            response->maximum = 0;
            response->average = 0;
        } else {

            int cont = 0;
            float sum = 0.0;
            float max = 0;
            float min = INFINITY;

            while (cont < tam){
                sum += valores[cont];
                

                if (valores[cont] > max){
                    max = valores[cont];
                }
                if (valores[cont] < min){
                    min = valores[cont];
                }

                cont++;
            }

            response->minimum = min;
            response->maximum = max;
            response->average = sum / tam;
        }
        

        RCLCPP_INFO(
            this->get_logger(),
            "As estatísticas desses valores são: Mínimo = %f | Máximo = %f | Média = %f",
            response->minimum,
            response->maximum,
            response->average
        );
        
    }

    rclcpp::Service<ArrayStatistics>::SharedPtr service_;
};

int main(int argc, char* argv[]){

    rclcpp::init(argc, argv);

    auto node = std::make_shared<ArrayStatisticsServer>();

    rclcpp::spin(node);

    rclcpp::shutdown();

    return 0;
}