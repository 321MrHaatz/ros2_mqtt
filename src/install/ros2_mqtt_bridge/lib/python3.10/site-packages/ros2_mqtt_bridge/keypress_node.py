# mqtt_bridge/keypress_node.py

import rclpy
from rclpy.node import Node
from std_msgs.msg import String
from pynput import keyboard

class KeyPressNode(Node):
    def __init__(self):
        super().__init__('key_press_node')
        self.publisher_ = self.create_publisher(String, 'mqtt_topic', 10)
        self.get_logger().info("Key Press Node started. Listening for key presses...")

    def on_press(self, key):
        try:
            if key == keyboard.Key.space:
                # Handle space bar separately
                self.publish_message('S')
            elif key == keyboard.Key.up:
                self.publish_message('F')
            elif key == keyboard.Key.down:
                self.publish_message('B')
            elif key == keyboard.Key.left:
                self.publish_message('L')
            elif key == keyboard.Key.right:
                    self.publish_message('R')
        except Exception as e:
            self.get_logger().error(f"Error processing key press: {e}")

    def publish_message(self, message):
        msg = String()
        msg.data = message
        self.publisher_.publish(msg)
        self.get_logger().info(f'Published message: {message}')

def main(args=None):
    rclpy.init(args=args)
    node = KeyPressNode()
    listener = keyboard.Listener(on_press=node.on_press)
    listener.start()
    rclpy.spin(node)
    listener.stop()
    node.destroy_node()
    rclpy.shutdown()

if __name__ == '__main__':
    main()
