#!/usr/bin/env python3

import rclpy
from rclpy.node import Node

from robot_interfaces.msg import RobotState


class RobotStatePublisher(Node):

    def __init__(self):
        super().__init__("robot_state_publisher")

        self.publisher_ = self.create_publisher(
            RobotState,
            "/robot_state",
            10
        )

        self.timer = self.create_timer(
            1.0,
            self.publish_state
        )

    def publish_state(self):

        msg = RobotState()

        msg.vel_lin = 1.5
        msg.vel_ang = 0.5
        msg.bat_percent = 80.0
        msg.indicador = "on"

        self.publisher_.publish(msg)

        self.get_logger().info(
            f'Linear: {msg.vel_lin} m/s | '
            f'Angular: {msg.vel_ang} rad/s | '
            f'Bateria: {msg.bat_percent}% | '
            f'Habilitado: {msg.indicador}'
        )


def main():

    rclpy.init()

    node = RobotStatePublisher()

    rclpy.spin(node)

    node.destroy_node()

    rclpy.shutdown()


if __name__ == "__main__":
    main()
