#include "rclcpp/rclcpp.hpp"
#include "custom_interfaces/srv/linear_regression.hpp"

#include <cmath>


using LinearRegression = custom_interfaces::srv::LinearRegression;

class LinearRegressionServer : public rclcpp::Node 
{
public:

    LinearRegressionServer() : Node("linear_regression_server")
    {
        service_ = this->create_service<LinearRegression>(
            "linear_regression",
            std::bind(
                &LinearRegressionServer::callback,
                this,
                std::placeholders::_1,
                std::placeholders::_2
            )
        );

        RCLCPP_INFO(
            this->get_logger(),
            "Servidor de mínimos quadrados iniciado."
        );
    }

private:

    void callback(
        const std::shared_ptr<LinearRegression::Request> request,
        std::shared_ptr<LinearRegression::Response> response
    ) {


        auto x = request->x;
        auto y = request->y;
        int tam_x = x.size();
        int tam_y = y.size();

        int cont_x = 0;
        bool igual = true;

        while (cont_x < tam_x - 1){
            if (x[cont_x] != x[cont_x+1]) {
                igual = false;
            }
            cont_x++;
        }

        if ( (tam_x != tam_y) || (igual == true) ){

            response->slope = 0.0;
            response->intercept = 0.0;
            response->r_squared = 0.0;

        } else {

            float sum_xy = 0.0;
            float sum_x = 0.0;
            float sum_y = 0.0;
            float sum_x2 = 0.0;
            int cont1 = 0;

            while (cont1 < tam_x){
                sum_xy += x[cont1] * y[cont1];
                sum_x += x[cont1];
                sum_y += y[cont1];
                sum_x2 += pow(x[cont1], 2);

                cont1++;
            }


            float m = ( (tam_x * sum_xy) - (sum_x * sum_y) ) / ( (tam_x * sum_x2) - pow(sum_x, 2) );
            float b = ( sum_y - (m * sum_x) ) / tam_x;
            

            float sum_func = 0.0;
            float sum_desvio = 0.0;
            float media_y = sum_y / tam_y;
            int cont2 = 0;

            while (cont2 < tam_x){
                float func = y[cont2] - ( (m * x[cont2]) + b );
                sum_func += pow(func, 2);

                float desvio = y[cont2] - media_y;
                sum_desvio += pow(desvio, 2);

                cont2++;
            }


            float r2 = 1.0 - (sum_func / sum_desvio);

            response->slope = m;
            response->intercept = b;
            response->r_squared = r2;

        }
        

        RCLCPP_INFO(
            this->get_logger(),
            "Os pares de pontos normalizados tem as seguintes constantes: Slope = %f | Intercept = %f | R_squared = %f",
            response->slope,
            response->intercept,
            response->r_squared
        );
        
    }

    rclcpp::Service<LinearRegression>::SharedPtr service_;
};

int main(int argc, char* argv[]){

    rclcpp::init(argc, argv);

    auto node = std::make_shared<LinearRegressionServer>();

    rclcpp::spin(node);

    rclcpp::shutdown();

    return 0;
}