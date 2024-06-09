# ros2_mqtt
Nodos de ROS 2 Humble para el manejo manual de un robot de plataforma diferencial, por medio de un servidor MQTT. Con un codigo de Arduino IDE para una ESP32.

1. Instalar librerias necesarias(Conexion MQTT y entrada de teclado en Python)

```bash
pip install paho-mqtt
pip install pynput
```
2. Crear workspace 
```bash
mkdir /ros2_mqtt_bridge/src
cd /ros2_mqtt_bridge/src
ros2 pkg create --build-type ament_python ros2_mqtt_bridge
```

