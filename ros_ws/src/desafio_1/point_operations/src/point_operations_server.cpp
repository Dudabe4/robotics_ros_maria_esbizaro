#include "rclcpp/rclcpp.hpp"
#include "custom_interfaces/srv/point_operations.hpp"

#include "custom_interfaces/msg/coordinate_conversions.hpp"
#include "custom_interfaces/msg/point_transformations.hpp"

#include <cmath>

using PointOperations = custom_interfaces::srv::PointOperations;

using CoordinateConversions = custom_interfaces::msg::CoordinateConversions;
using PointTransformations = custom_interfaces::msg::PointTransformations;


class PointOperationsServer : public rclcpp::Node
{
public:

    PointOperationsServer()
        : Node("point_operations_server")
    {

        service_ = this->create_service<PointOperations>(
            "point_operations",
            std::bind(
                &PointOperationsServer::callback,
                this,
                std::placeholders::_1,
                std::placeholders::_2
            )
        );

        RCLCPP_INFO(
            this->get_logger(),
            "Servidor de operações em um ponto iniciado."
        );
    }

private:

    CoordinateConversions conversao;
    PointTransformations transformacao;

    void callback(
        const std::shared_ptr<PointOperations::Request> request,
        std::shared_ptr<PointOperations::Response> response)
    {

        auto ponto = request->point;
        float ponto_x = ponto.x;
        float ponto_y = ponto.y;
        float ponto_z = ponto.z;

        conversion(ponto_x, ponto_y, ponto_z);
        transform(ponto_x, ponto_y, ponto_z);

        response->results1 = conversao;
        response->results2 = transformacao;

        RCLCPP_INFO(
            this->get_logger(),
            "Respostas-> sph_r = %f , sph_theta = %f , sph_phi = %f , cyl_rho = %f , cyl_phi = %f , cyl_z = %f , azimuth = %f , elevation = %f || transf_x = %f , transf_y = %f , transf_z = %f , scl_x = %f , scl_y = %f , scl_z = %f ",
            conversao.spherical_r,
            conversao.spherical_theta,
            conversao.spherical_phi,
            conversao.cylindrical_rho,
            conversao.cylindrical_phi,
            conversao.cylindrical_z,
            conversao.azimuth,
            conversao.elevation,
            transformacao.rigid_transform.x,
            transformacao.rigid_transform.y,
            transformacao.rigid_transform.z,
            transformacao.scaled.x,
            transformacao.scaled.y,
            transformacao.scaled.z
        );
    }

    void conversion(float ponto_x, float ponto_y, float ponto_z){

        conversao.spherical_r = std::sqrt( pow(ponto_x, 2) + pow(ponto_y, 2) + pow(ponto_z, 2) );
        
        if (conversao.spherical_r == 0.0){
            conversao.spherical_theta = 0.0;
        } else {
            conversao.spherical_theta = std::acos( (ponto_z / conversao.spherical_r) );
        }

        conversao.spherical_phi = std::atan2(ponto_y, ponto_x);
        conversao.cylindrical_rho = std::sqrt( pow(ponto_x, 2) + pow(ponto_y, 2) );
        conversao.cylindrical_phi = conversao.spherical_phi;
        conversao.cylindrical_z = ponto_z;
        conversao.azimuth = conversao.spherical_phi;
        conversao.elevation = std::atan2(ponto_z, conversao.cylindrical_rho);

    }

    void transform(float ponto_x, float ponto_y, float ponto_z){

        float p[3] = {ponto_x, ponto_y, ponto_z};
        float rp[3] = {0.0, 0.0, 0.0};
        float t[3] = {1.0, 2.0, 3.0};
        float rigid_transform[3] = {0.0, 0.0, 0.0};
        float scaled[3] = {0.0, 0.0, 0.0};


        float R[3][3] = {
            {-1.0, 0.0, 0.0},
            {0.0, 0.0, 1.0},
            {0.0, 1.0, 0.0}
        };


        for (int i = 0; i < 3; i++){
            for (int j = 0; j < 3; j++){
                rp[i] += R[i][j] * p[j];
            }
        }

        for (int i = 0; i < 3; i++){
            rigid_transform[i] = rp[i] + t[i];
        }


        transformacao.rigid_transform.x = rigid_transform[0];
        transformacao.rigid_transform.y = rigid_transform[1];
        transformacao.rigid_transform.z = rigid_transform[2];

        
        float S[3][3] = {
            {2.0, 0, 0},
            {0, 0.5, 0},
            {0, 0, 1.5}
        };


        for (int i = 0; i < 3; i++){
            scaled[i] = S[i][i] * p[i];
        }


        transformacao.scaled.x = scaled[0];
        transformacao.scaled.y = scaled[1];
        transformacao.scaled.z = scaled[2];

    }

    rclcpp::Service<PointOperations>::SharedPtr service_;
};

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);

    auto node = std::make_shared<PointOperationsServer>();

    rclcpp::spin(node);

    rclcpp::shutdown();

    return 0;
}