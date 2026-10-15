#include "rclcpp/rclcpp.hpp"
#include "custom_interfaces/srv/subtract_two_ints.hpp"

using SubtractTwoInts = custom_interfaces::srv::SubtractTwoInts;

class SubtractIntsServer : public rclcpp::Node
{
public:

    SubtractIntsServer()
        : Node("subtract_ints_server")
    {
        service_ = this->create_service<SubtractTwoInts>(
            "subtract_two_ints",
            std::bind(
                &SubtractIntsServer::callback,
                this,
                std::placeholders::_1,
                std::placeholders::_2
            )
        );

        RCLCPP_INFO(
            this->get_logger(),
            "Servidor de subtracao iniciado."
        );
    }

private:

    void callback(
        const std::shared_ptr<SubtractTwoInts::Request> request,
        std::shared_ptr<SubtractTwoInts::Response> response)
    {
        response->difference = request->a - request->b;

        RCLCPP_INFO(
            this->get_logger(),
            "Recebido: %d - %d = %d",
            request->a,
            request->b,
            response->difference
        );
    }

    rclcpp::Service<SubtractTwoInts>::SharedPtr service_;
};

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);

    auto node = std::make_shared<SubtractIntsServer>();

    rclcpp::spin(node);

    rclcpp::shutdown();

    return 0;
}
