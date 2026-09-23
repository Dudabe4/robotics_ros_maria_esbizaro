#!/usr/bin/env python3

import rclpy
from rclpy.node import Node

from robot_interfaces.msg import SensorInfo


msg = SensorInfo()
msg.name = "SensorA"
msg.freq = 0.5
msg.indicador = "on"
msg.count = 1

class SensorInfoPublisher(Node):

    def __init__(self):
        super().__init__("sensor_info_publisher")

        self.publisher_ = self.create_publisher(
            SensorInfo,
            "/sensor_info",
            10
        )

        self.timer = self.create_timer(
            1.0,
            self.publish_info
        )

    def publish_info(self):

        self.publisher_.publish(msg)

        self.get_logger().info(
            f'Nome: {msg.name} | '
            f'Frequencia: {msg.freq} Hz | '
            f'Habilitado: {msg.indicador} | '
            f'Contador: {msg.count}'
        )

        msg.count += 1


def main():

    rclpy.init()

    node = SensorInfoPublisher()

    rclpy.spin(node)

    node.destroy_node()

    rclpy.shutdown()


if __name__ == "__main__":
    main()
