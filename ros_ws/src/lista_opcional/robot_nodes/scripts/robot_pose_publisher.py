#!/usr/bin/env python3

import rclpy
from rclpy.node import Node

from robot_interfaces.msg import RobotPose


class RobotPosePublisher(Node):

    def __init__(self):
        super().__init__('robot_pose_publisher')

        self.publisher = self.create_publisher(
            RobotPose,
            '/robot_pose',
            10
        )

        self.timer = self.create_timer(
            10.0,
            self.publish_pose
        )

    def publish_pose(self):
        msg = RobotPose()

        msg.pose.position.x = 1.0
        msg.pose.position.y = 2.0
        msg.pose.position.z = 0.0

        msg.pose.orientation.x = 0.0
        msg.pose.orientation.y = 0.0
        msg.pose.orientation.z = 0.0
        msg.pose.orientation.w = 1.0

        msg.velocity.linear.x = 1.5
        msg.velocity.linear.y = 0.0
        msg.velocity.linear.z = 0.0

        msg.velocity.angular.x = 0.0
        msg.velocity.angular.y = 0.0
        msg.velocity.angular.z = 0.5

        self.publisher.publish(msg)

        self.get_logger().info(
            f'Pose: x={msg.pose.position.x}, '
            f'y={msg.pose.position.y} | '
            f'Velocidade: linear={msg.velocity.linear.x}, '
            f'angular={msg.velocity.angular.z}'
        )

def main(args=None):
    rclpy.init(args=args)

    node = RobotPosePublisher()

    rclpy.spin(node)

    node.destroy_node()
    rclpy.shutdown()


if __name__ == '__main__':
    main()