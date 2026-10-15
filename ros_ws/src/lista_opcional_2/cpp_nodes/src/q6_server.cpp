#include "rclcpp/rclcpp.hpp"
#include "custom_interfaces/srv/dot_product.hpp"

#include "geometry_msgs/msg/vector3.hpp"


using DotProduct = custom_interfaces::srv::DotProduct;

class DotProductServer : public rclcpp::Node 
{
public:

    DotProductServer() : Node("dot_product_server")
    {
        service_ = this->create_service<DotProduct>(
            "dot_product",
            std::bind(
                &DotProductServer::callback,
                this,
                std::placeholders::_1,
                std::placeholders::_2
            )
        );

        RCLCPP_INFO(
            this->get_logger(),
            "Servidor de produto escalar iniciado."
        );
    }

private:

    void callback(
        const std::shared_ptr<DotProduct::Request> request,
        std::shared_ptr<DotProduct::Response> response
    ) {

        geometry_msgs::msg::Vector3 vetor_a = request->vector_a;
        geometry_msgs::msg::Vector3 vetor_b = request->vector_b;

        float x = vetor_a.x * vetor_b.x;
        float y = vetor_a.y * vetor_b.y;
        float z = vetor_a.z * vetor_b.z;

        response->result = x + y + z;
        
        RCLCPP_INFO(
            this->get_logger(),
            "O produto escalar dos vetores é de %f",
            response->result
        );
        
    }

    rclcpp::Service<DotProduct>::SharedPtr service_;
};

int main(int argc, char* argv[]){

    rclcpp::init(argc, argv);

    auto node = std::make_shared<DotProductServer>();

    rclcpp::spin(node);

    rclcpp::shutdown();

    return 0;
}