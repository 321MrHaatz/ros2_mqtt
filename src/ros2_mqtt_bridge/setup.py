from setuptools import setup

package_name = 'ros2_mqtt_bridge'

setup(
    name=package_name,
    version='0.1.0',
    packages=[package_name],
    py_modules=[
        'ros2_mqtt_bridge.mqtt_bridge_node',
        'ros2_mqtt_bridge.keypress_node',  # Add this line
    ],
    install_requires=['setuptools', 'pynput'],  # Add 'pynput' here
    zip_safe=True,
    maintainer='your_name',
    maintainer_email='your_email@example.com',
    description='ROS 2 to MQTT bridge',
    license='Apache License 2.0',
    tests_require=['pytest'],
    entry_points={
        'console_scripts': [
            'mqtt_bridge_node = ros2_mqtt_bridge.mqtt_bridge_node:main',
            'keypress_node = ros2_mqtt_bridge.keypress_node:main',  # Add this line
        ],
    },
)
