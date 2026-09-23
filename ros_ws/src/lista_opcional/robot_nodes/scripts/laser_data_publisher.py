#!/usr/bin/env python3

import rclpy
from rclpy.node import Node

from robot_interfaces.msg import LaserData

class LaserDataPublisher(Node):

    def __init__(self):
        super().__init__('laser_data_publisher')

        self.publisher = self.create_publisher(
            LaserData,
            '/laser_data',
            10
        )

        self.timer = self.create_timer(
            1.0,
            self.publish_laser_data
        )

    def publish_laser_data(self):
        msg = LaserData()

        msg.dists = [
            2.5,
            1.8,
            3.2,
            0.9,
            4.1,
            2.7,
            1.2,
            3.8,
            2.0,
            1.5
        ]

        msg.angle_increment = 0.1
        msg.angle_i = -0.45
        msg.angle_f = 0.45

        self.publisher.publish(msg)

        self.get_logger().info(
            f'Publicadas {len(msg.dists)} medições do LiDAR'
        )

def main(args=None):
    rclpy.init(args=args)

    node = LaserDataPublisher()

    rclpy.spin(node)

    node.destroy_node()
    rclpy.shutdown()


if __name__ == '__main__':
    main()