import rclpy
from rclpy.node import Node
from std_msgs.msg import String
import paho.mqtt.client as mqtt

class MqttBridgeNode(Node):
    def __init__(self):
        super().__init__('mqtt_bridge_node')
        
        # ROS 2 publisher and subscriber
        self.publisher_ = self.create_publisher(String, 'ros_topic', 10)
        self.subscription = self.create_subscription(String, 'mqtt_topic', self.listener_callback, 10)
        
        # MQTT client setup
        self.mqtt_client = mqtt.Client()
        self.mqtt_client.on_connect = self.on_connect
        self.mqtt_client.on_message = self.on_message
        
        # Connect to MQTT broker
        self.mqtt_client.connect("broker.emqx.io", 1883, 60)
        self.mqtt_client.loop_start()

    def on_connect(self, client, userdata, flags, rc):
        self.get_logger().info("Connected to MQTT broker")
        client.subscribe("esp32_ros")

    def on_message(self, client, userdata, msg):
        self.get_logger().info(f"Received message: {msg.payload.decode()} on topic: {msg.topic}")
        ros_msg = String()
        ros_msg.data = msg.payload.decode()
        self.publisher_.publish(ros_msg)

    def listener_callback(self, msg):
        self.get_logger().info(f"Received ROS message: {msg.data}")
        self.mqtt_client.publish("esp32_ros", msg.data)

def main(args=None):
    rclpy.init(args=args)
    node = MqttBridgeNode()
    rclpy.spin(node)
    node.destroy_node()
    rclpy.shutdown()

if __name__ == '__main__':
    main()
