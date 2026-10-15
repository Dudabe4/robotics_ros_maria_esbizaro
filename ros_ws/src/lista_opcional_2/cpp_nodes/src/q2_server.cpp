#include "rclcpp/rclcpp.hpp"
#include "custom_interfaces/srv/is_even.hpp"

#include <string>
#include <sstream>

using IsEven = custom_interfaces::srv::IsEven;

class IsEvenServer : public rclcpp::Node 
{
public:

    IsEvenServer() : Node("is_even_server")
    {
        service_ = this->create_service<IsEven>(
            "is_even",
            std::bind(
                &IsEvenServer::callback,
                this,
                std::placeholders::_1,
                std::placeholders::_2
            )
        );

        RCLCPP_INFO(
            this->get_logger(),
            "Servidor de par ou impar iniciado."
        );
    }

private:

    void callback(
        const std::shared_ptr<IsEven::Request> request,
        std::shared_ptr<IsEven::Response> response
    ) {

        if ( (request->number)%2 == 0 ){
            response->is_even = true;
        } else {
            response->is_even = false;
        }

        std::stringstream ss;
        ss << std::boolalpha << response->is_even;

        RCLCPP_INFO(
            this->get_logger(),
            "O numero %d eh par? %s",
            request->number,
            ss.str().c_str()
        );
    }

    rclcpp::Service<IsEven>::SharedPtr service_;
};

int main(int argc, char* argv[]){

    rclcpp::init(argc, argv);

    auto node = std::make_shared<IsEvenServer>();

    rclcpp::spin(node);

    rclcpp::shutdown();

    return 0;
}