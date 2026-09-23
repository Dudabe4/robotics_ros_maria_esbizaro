#!/usr/bin/env python3

import rclpy
from rclpy.node import Node

from robot_interfaces.msg import VelocityHistory


class VelocityHistoryPublisher(Node):

    def __init__(self):
        super().__init__('velocity_history_publisher')

        self.publisher = self.create_publisher(
            VelocityHistory,
            '/velocity_history',
            10
        )

        self.timer = self.create_timer(
            1.0,
            self.publish_history
        )

    def publish_history(self):
        msg = VelocityHistory()

        msg.linear_velocities = [1.0, 1.5, 2.0, 2.5]
        msg.angular_velocities = [0.1, 0.2, 0.3, 0.4]
        msg.sample_count = 4

        self.publisher.publish(msg)

        self.get_logger().info('Histórico de velocidades publicado')


def main(args=None):
    rclpy.init(args=args)

    node = VelocityHistoryPublisher()

    rclpy.spin(node)

    node.destroy_node()
    rclpy.shutdown()


if __name__ == '__main__':
    main()