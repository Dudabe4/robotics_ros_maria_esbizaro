import rclpy
from rclpy.node import Node
from std_msgs.msg import String

class FirstNode(Node):
    def __init__(self):
        super().__init__("first_node")
        self.get_logger().info("Node started!")

        self.publisher_ = self.create_publisher(
            String,
            "text",
            10
        )

        self.timer = self.create_timer(
            1.0,
            self.timer_callback
        )

    def timer_callback(self):
        msg = String()
        msg.data = "Hello ROS 2!"
        self.publisher_.publish(msg)
        self.get_logger().info(
            f"Publishing: {msg.data}"
        )

def main():
    rclpy.init() # Inicia o ROS2 e prepara o ambiente necessário
    node = FirstNode() # Cria a instância do nó
    rclpy.spin(node)
    # O spin() entra em um loop de processamento de eventos, permitindo que o ROS 2 execute
    node.destroy_node()
    rclpy.shutdown()
    # O método destroy_node() remove o nó corretamente, enquanto shutdown() finaliza a comu

if __name__ == "__main__":
    main()