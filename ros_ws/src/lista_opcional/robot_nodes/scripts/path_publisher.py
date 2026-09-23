#!/usr/bin/env python3

import rclpy
from rclpy.node import Node

from robot_interfaces.msg import Path
from geometry_msgs.msg import Pose


class PathPublisher(Node):

    def __init__(self):
        super().__init__('path_publisher')

        self.publisher = self.create_publisher(
            Path,
            '/path',
            10
        )

        self.timer = self.create_timer(
            10.0,
            self.publish_path
        )

    def publish_path(self):
        msg = Path()

        pose1 = Pose()
        pose1.position.x = 0.0
        pose1.position.y = 0.0
        pose1.position.z = 0.0
        pose1.orientation.w = 1.0
        msg.poses.append(pose1)

        pose2 = Pose()
        pose2.position.x = 1.0
        pose2.position.y = 0.5
        pose2.position.z = 0.0
        pose2.orientation.w = 1.0
        msg.poses.append(pose2)

        pose3 = Pose()
        pose3.position.x = 2.0
        pose3.position.y = 1.0
        pose3.position.z = 0.0
        pose3.orientation.w = 1.0
        msg.poses.append(pose3)

        pose4 = Pose()
        pose4.position.x = 3.0
        pose4.position.y = 1.5
        pose4.position.z = 0.0
        pose4.orientation.w = 1.0
        msg.poses.append(pose4)

        pose5 = Pose()
        pose5.position.x = 4.0
        pose5.position.y = 2.0
        pose5.position.z = 0.0
        pose5.orientation.w = 1.0
        msg.poses.append(pose5)

        self.publisher.publish(msg)

        self.get_logger().info(
            f'Trajetoria publicada com {len(msg.poses)} poses'
        )

def main(args=None):
    rclpy.init(args=args)

    node = PathPublisher()

    rclpy.spin(node)

    node.destroy_node()
    rclpy.shutdown()


if __name__ == '__main__':
    main()