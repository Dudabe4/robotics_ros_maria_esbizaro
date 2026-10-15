#include "rclcpp/rclcpp.hpp"
#include "custom_interfaces/srv/convert_temperature.hpp"

using ConvertTemperature = custom_interfaces::srv::ConvertTemperature;

class ConvertTemperatureServer : public rclcpp::Node 
{
public:

    ConvertTemperatureServer() : Node("convert_temperature_server")
    {
        service_ = this->create_service<ConvertTemperature>(
            "convert_temperature",
            std::bind(
                &ConvertTemperatureServer::callback,
                this,
                std::placeholders::_1,
                std::placeholders::_2
            )
        );

        RCLCPP_INFO(
            this->get_logger(),
            "Servidor de conversão de temperatura iniciado."
        );
    }

private:

    void callback(
        const std::shared_ptr<ConvertTemperature::Request> request,
        std::shared_ptr<ConvertTemperature::Response> response
    ) {

        response->fahrenheit = ( ( (request->celsius)*9 )/5 ) + 32;

        RCLCPP_INFO(
            this->get_logger(),
            "O numero %f °C em fahrenheit é %f F",
            request->celsius,
            response->fahrenheit
        );
    }

    rclcpp::Service<ConvertTemperature>::SharedPtr service_;
};

int main(int argc, char* argv[]){

    rclcpp::init(argc, argv);

    auto node = std::make_shared<ConvertTemperatureServer>();

    rclcpp::spin(node);

    rclcpp::shutdown();

    return 0;
}