#include <algorithm>
#include <cmath>
#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist_stamped.hpp"
#include "sensor_msgs/msg/joint_state.hpp"

using std::placeholders::_1;

using namespace std::chrono_literals;


class CinematicaAckermannNode : public rclcpp::Node
{
public:
    CinematicaAckermannNode() : Node("cinematica_ackermann")
    {

        RCLCPP_INFO(
            this->get_logger(),
            "Nó de cinemática Ackermann iniciado!"
        );
    
        // Declara os parâmetros geométricos e o tempo de validade dos comandos.
        this->declare_parameter<double>("wheel_radius", 0.0);
        this->declare_parameter<double>("wheel_base", 0.0);
        this->declare_parameter<double>("wheel_separation", 0.0);
        this->declare_parameter<double>("max_steering_angle", 0.0);
        this->declare_parameter<double>("command_timeout", 0.5);


        // Publica os ângulos de esterçamento e as velocidades das rodas.
        publisher_ = this->create_publisher<sensor_msgs::msg::JointState>(
            "/ackermann/rodas", 
            10
        );

        // Recebe os comandos de velocidade do robô.
        subscription_ = this->create_subscription<geometry_msgs::msg::TwistStamped>(
            "/cmd_vel", 
            10,
            std::bind(
                &CinematicaAckermannNode::callback,
                this,
                std::placeholders::_1
            )
        );

        // Verifica periodicamente se o último comando expirou.
        timeout_timer_ = this->create_wall_timer(
            50ms, 
            std::bind(
                &CinematicaAckermannNode::verificarTimeout, 
                this
            )
        );

    }

private:
    sensor_msgs::msg::JointState estadoZero() {

        sensor_msgs::msg::JointState estado;

        estado.header.stamp = this->now();

        estado.name = {
            "front_left_steering_joint", "front_right_steering_joint",
            "rear_left_wheel_joint", "rear_right_wheel_joint"};

        estado.position = {0.0, 0.0, 0.0, 0.0};
        estado.velocity = {0.0, 0.0, 0.0, 0.0};

        return estado;
    }

    void publicarZero() {

        // Interrompe as rodas e retorna o esterçamento à posição neutra.
        publisher_->publish(estadoZero());
        comando_valido_ = false;
    }

    void callback(const geometry_msgs::msg::TwistStamped::SharedPtr msg) {

        const double radius = this->get_parameter("wheel_radius").as_double();
        const double wheelbase = this->get_parameter("wheel_base").as_double();
        const double separation = this->get_parameter("wheel_separation").as_double();
        const double MAX_ANGLE = this->get_parameter("max_steering_angle").as_double();
        const double timeout = this->get_parameter("command_timeout").as_double();


        double v = msg->twist.linear.x;
        double omega = msg->twist.angular.z;


        // Calcula a idade do comando usando o timestamp da mensagem.
        const rclcpp::Time stamp(msg->header.stamp, this->get_clock()->get_clock_type());
        const rclcpp::Time agora = this->now();
        const double idade = (agora - stamp).seconds();
        

        if (!std::isfinite(timeout) || timeout <= 0.0 || stamp.nanoseconds() == 0 || !std::isfinite(idade) || idade < -0.05 || idade > timeout) {
            RCLCPP_WARN(
                this->get_logger(),
                "Comando com timestamp invalido ou antigo."
            );

            publicarZero();

            return;
        }

        // Impede cálculos com dimensões físicas inválidas.
        if (!std::isfinite(radius) || !std::isfinite(wheelbase) || !std::isfinite(separation) || !std::isfinite(MAX_ANGLE) || radius <= 0.0 || wheelbase <= 0.0 || separation <= 0.0 || MAX_ANGLE <= 0.0) {
            RCLCPP_WARN(
                this->get_logger(),
                "Configurações incorretas dos parametros do veiculo."
            );

            publicarZero();

            return;
        }


        // O modelo Ackermann não permite girar sobre o próprio eixo.
        if (std::abs(v) < 1e-6 && std::abs(omega) > 1e-6) {
            RCLCPP_WARN(
                this->get_logger(),
                "O modelo Ackermann nao permite giro no proprio eixo."
            );

            publicarZero();

            return;
        }

        auto estado = estadoZero();
        estado.header.stamp = msg->header.stamp;

        
        // Calcula as velocidades lineares das rodas traseiras.
        double half_separation = separation / 2.0;

        double v_left = v - omega * half_separation;
        double v_right = v + omega * half_separation;


        double left_angle = 0.0;
        double right_angle = 0.0;

        // Calcula e limita os ângulos de esterçamento dianteiros.
        if (std::abs(v) > 1e-6 && std::abs(omega) > 1e-6) {

            left_angle = std::atan2( (wheelbase * omega), (v_left) );
            right_angle = std::atan2( (wheelbase * omega), (v_right) );

            left_angle = std::clamp(left_angle, -MAX_ANGLE, MAX_ANGLE);
            right_angle = std::clamp(right_angle, -MAX_ANGLE, MAX_ANGLE);
        }


        estado.name = {
            "front_left_steering_joint",
            "front_right_steering_joint",
            "rear_left_wheel_joint",
            "rear_right_wheel_joint"
        };

        estado.position[0] = left_angle;
        estado.position[1] = right_angle;

        // Converte as velocidades lineares em velocidades angulares (rad/s).
        estado.velocity[2] = v_left / radius;
        estado.velocity[3] = v_right / radius;


        publisher_->publish(estado);
        comando_valido_ = true;
        ultimo_stamp_ = stamp;

        RCLCPP_INFO(
            this->get_logger(),
            "Esterçamento: esquerda=%.3f rad, direita=%.3f rad | Velocidade: esquerda=%.3f rad/s, direita=%.3f rad/s",
            estado.position[0], 
            estado.position[1], 
            estado.velocity[2], 
            estado.velocity[3]
        );
    }

    void verificarTimeout() {

        if (!comando_valido_) {
            return;
        }

        const double timeout = this->get_parameter("command_timeout").as_double();

        if (timeout <= 0.0 || (this->now() - ultimo_stamp_).seconds() > timeout) {

            RCLCPP_WARN(
                this->get_logger(), 
                "Comando /cmd_vel expirou; publicando velocidades zero."
            );

            publicarZero();
        }
  }

    rclcpp::Publisher<sensor_msgs::msg::JointState>::SharedPtr publisher_;
    rclcpp::Subscription<geometry_msgs::msg::TwistStamped>::SharedPtr subscription_;
    rclcpp::TimerBase::SharedPtr timeout_timer_;
    bool comando_valido_{false};
    rclcpp::Time ultimo_stamp_{0, 0, RCL_ROS_TIME};

};

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);

    rclcpp::spin(std::make_shared<CinematicaAckermannNode>());

    rclcpp::shutdown();

    return 0;
}