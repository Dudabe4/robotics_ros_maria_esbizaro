#include "rclcpp/rclcpp.hpp"
#include "custom_interfaces/srv/point_operations.hpp"

#include "custom_interfaces/msg/coordinate_conversions.hpp"
#include "custom_interfaces/msg/point_transformations.hpp"

#include "geometry_msgs/msg/vector3.hpp"

#include <chrono>
#include <cstdlib>
#include <memory>

using namespace std::chrono_literals;

using PointOperations = custom_interfaces::srv::PointOperations;

using CoordinateConversions = custom_interfaces::msg::CoordinateConversions;
using PointTransformations = custom_interfaces::msg::PointTransformations;

using Vector3 = geometry_msgs::msg::Vector3;


int main(int argc, char* argv[])
{
    rclcpp::init(argc, argv);

    if (argc != 4){
        
        RCLCPP_ERROR(
            rclcpp::get_logger("rclcpp"),
            "Uso ros2 run point_operations "
            "point_operations_client <x> <y> <z>"
        );

        rclcpp::shutdown();
        return 1;
    }

    auto node = rclcpp:: Node::make_shared("point_operations_client");

    auto client = node->create_client<PointOperations>("point_operations");

    auto point_publisher = node->create_publisher<Vector3>("point", 10);
    auto conversion_publisher = node->create_publisher<CoordinateConversions>("coordinate_conversions", 10);
    auto transformation_publisher = node->create_publisher<PointTransformations>("point_transformations", 10);

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

    auto request = std::make_shared<PointOperations::Request>();

    float x = atof(argv[1]);
    float y = atof(argv[2]);
    float z = atof(argv[3]);

    auto ponto = Vector3();
    ponto.x = x;
    ponto.y = y;
    ponto.z = z;

    request->point = ponto;

    auto future = client->async_send_request(request);

    auto status = 
        rclcpp::spin_until_future_complete(
            node,
            future
        );

    if (status == rclcpp::FutureReturnCode::SUCCESS){
        
        auto response = future.get();

        RCLCPP_INFO(
            node->get_logger(),
            "Respostas-> sph_r = %f , sph_theta = %f , sph_phi = %f , cyl_rho = %f , cyl_phi = %f , cyl_z = %f , azimuth = %f , elevation = %f || transf_x = %f , transf_y = %f , transf_z = %f , scl_x = %f , scl_y = %f , scl_z = %f ",
            response->results1.spherical_r,
            response->results1.spherical_theta,
            response->results1.spherical_phi,
            response->results1.cylindrical_rho,
            response->results1.cylindrical_phi,
            response->results1.cylindrical_z,
            response->results1.azimuth,
            response->results1.elevation,
            response->results2.rigid_transform.x,
            response->results2.rigid_transform.y,
            response->results2.rigid_transform.z,
            response->results2.scaled.x,
            response->results2.scaled.y,
            response->results2.scaled.z
        );

        auto timer = node->create_wall_timer(
            1s,
            [ponto, response,
             point_publisher,
             conversion_publisher,
             transformation_publisher]() {
                point_publisher->publish(ponto);
                conversion_publisher->publish(response->results1);
                transformation_publisher->publish(response->results2);
            }
        );

        rclcpp::spin(node);

    } else {

        RCLCPP_ERROR(
            node->get_logger(),
            "Falha ao chamar o serviço."
        );
    }

    rclcpp::shutdown();

    return 0;
}