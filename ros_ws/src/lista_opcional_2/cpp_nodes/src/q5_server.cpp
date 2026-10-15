#include "rclcpp/rclcpp.hpp"
#include "custom_interfaces/srv/reverse_string.hpp"

#include <string>
#include <algorithm>

using ReverseString = custom_interfaces::srv::ReverseString;

class ReverseStringServer : public rclcpp::Node 
{
public:

    ReverseStringServer() : Node("reverse_string_server")
    {
        service_ = this->create_service<ReverseString>(
            "reverse_string",
            std::bind(
                &ReverseStringServer::callback,
                this,
                std::placeholders::_1,
                std::placeholders::_2
            )
        );

        RCLCPP_INFO(
            this->get_logger(),
            "Servidor de inverter string iniciado."
        );
    }

private:

    void callback(
        const std::shared_ptr<ReverseString::Request> request,
        std::shared_ptr<ReverseString::Response> response
    ) {

        std::string str = request->text;
        std::reverse(str.begin(), str.end());
        response->reversed_text = str;
        
        RCLCPP_INFO(
            this->get_logger(),
            "O contrário da palavra %s é %s",
            (request->text).c_str(),
            (response->reversed_text).c_str()
        );
        
    }

    rclcpp::Service<ReverseString>::SharedPtr service_;
};

int main(int argc, char* argv[]){

    rclcpp::init(argc, argv);

    auto node = std::make_shared<ReverseStringServer>();

    rclcpp::spin(node);

    rclcpp::shutdown();

    return 0;
}