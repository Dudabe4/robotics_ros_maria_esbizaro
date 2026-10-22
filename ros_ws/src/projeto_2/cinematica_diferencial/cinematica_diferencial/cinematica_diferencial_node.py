import rclpy
from rclpy.node import Node
from geometry_msgs.msg import TwistStamped
from sensor_msgs.msg import JointState


class CinematicaDiferencialNode(Node):

    def __init__(self):
        super().__init__("cinematica_diferencial_node")

        # Declara os parâmetros geométricos e o tempo de validade dos comandos.
        self.declare_parameter('wheel_radius', 0.0)
        self.declare_parameter('wheel_separation', 0.0)
        self.declare_parameter('command_timeout', 0.5)


        # Recebe os comandos de velocidade do veículo.
        self.subscription = self.create_subscription(
            TwistStamped,
            "/cmd_vel",
            self.callback,
            10
        )

        # Publica as velocidades angulares das rodas.
        self.publisher_ = self.create_publisher(
            JointState,
            "/diferencial/rodas",
            10
        )


        self.last_valid_command_ns = None
        self.last_output_was_zero = False


        # Verifica periodicamente se o último comando expirou.
        self.timer = self.create_timer(
            0.05, 
            self.check_command_timeout
        )


        self.publish_zero()


    def publish_wheel_speeds(self, left_speed, right_speed, stamp=None):
        msg = JointState()

        msg.header.stamp = stamp if stamp is not None else self.get_clock().now().to_msg()

        msg.name = ['rear_left_wheel_joint', 'rear_right_wheel_joint']

        msg.velocity = [float(left_speed), float(right_speed)]

        self.publisher_.publish(msg)


    def publish_zero(self):

        # Interrompe as rodas quando não há um comando válido.
        self.publish_wheel_speeds(0.0, 0.0)

        self.last_output_was_zero = True


    def callback(self, msg):

        now_ns = self.get_clock().now().nanoseconds
        stamp_ns = (int(msg.header.stamp.sec) * 1000000000 + int(msg.header.stamp.nanosec))

        timeout = self.get_parameter('command_timeout').value
        radius = self.get_parameter('wheel_radius').value
        separation = self.get_parameter('wheel_separation').value

        v = msg.twist.linear.x
        omega = msg.twist.angular.z


        if timeout <= 0.0 or radius <= 0.0 or separation <= 0.0:
            self.get_logger().error(
                'Parâmetros inválidos: wheel_radius, wheel_separation e '
                'command_timeout devem ser positivos.'
            )

            self.last_valid_command_ns = None
            self.publish_zero()

            return

        # Calcula a idade do comando usando o timestamp da mensagem.        
        age_ns = now_ns - stamp_ns

        if stamp_ns == 0 or age_ns < -100000000 or age_ns > timeout * 1e9:
            self.get_logger().warn(
                'Comando rejeitado: timestamp zerado, futuro ou expirado.'
            )

            self.last_valid_command_ns = None
            self.publish_zero()

            return


        # Calcula as velocidades angulares das rodas esquerda e direita.
        left_speed = ( (v - (omega * separation / 2.0) ) ) / radius
        right_speed = ( (v + (omega * separation / 2.0) ) ) / radius

        self.last_valid_command_ns = now_ns
        self.last_output_was_zero = False

        self.publish_wheel_speeds(
            left_speed, right_speed, stamp=msg.header.stamp
        )

        self.get_logger().info(
            f'v = {v:.2f} m/s, '
            f'omega = {omega:.2f} rad/s, '
            f'left_speed = {left_speed:.2f} rad/s, '
            f'right_speed = {right_speed:.2f} rad/s'
        )

    def check_command_timeout(self):
       
        timeout = self.get_parameter('command_timeout').value
        now_ns = self.get_clock().now().nanoseconds

        # Publica velocidades zero quando não há comando válido ou ele expirou.
        if (timeout <= 0.0 or self.last_valid_command_ns is None or now_ns - self.last_valid_command_ns > timeout * 1e9):
            
            if not self.last_output_was_zero:
                self.publish_zero()

            if self.last_valid_command_ns is not None and timeout > 0.0:

                if now_ns - self.last_valid_command_ns > timeout * 1e9:
                    self.last_valid_command_ns = None


def main():

    rclpy.init()

    node = CinematicaDiferencialNode()

    rclpy.spin(node)

    node.destroy_node()

    rclpy.shutdown()


if __name__ == "__main__":
    main()
