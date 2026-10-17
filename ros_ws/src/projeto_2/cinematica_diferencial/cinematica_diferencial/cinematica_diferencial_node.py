import rclpy
from rclpy.node import Node
from std_msgs.msg import String


class NodeName(Node):

    def __init__(self):
        super().__init__("node_name")

        self.subscription = self.create_subscription(
            String,
            "topic_1",
            self.callback,
            10
        )

        self.publisher_ = self.create_publisher(
            String,
            "topic_2",
            10
        )

    def callback(self, msg):

        new_msg = String()

        new_msg.data = "Recebi: " + msg.data

        self.publisher_.publish(new_msg)


def main():

    rclpy.init()

    node = NodeName()

    rclpy.spin(node)

    node.destroy_node()

    rclpy.shutdown()


if __name__ == "__main__":
    main()
