import serial
import time
from paho.mqtt.client import Client
from paho.mqtt.enums import CallbackAPIVersion
import random
import sys

SERIAL_PORT = 'COM5' 
BAUD_RATE = 9600
BROKER = "broker.emqx.io"
LUMINOSITY_TOPIC = "laboratory/greenhouse/luminosity"
STATUS_TOPIC = "laboratory/greenhouse/sensor/status"

class SensorPublisher:
    def __init__(self, serial_port, baud_rate, broker):
        self.serial_port = serial_port
        self.baud_rate = baud_rate
        self.broker = broker
        
        try:
            self.ser = serial.Serial(serial_port, baud_rate, timeout=1)
            time.sleep(2) 
            print(f"Подключено к {serial_port}")
        except serial.SerialException as e:
            print(f"Ошибка подключения к {serial_port}: {e}")
            sys.exit(1)
        
        self.client = Client(
            callback_api_version=CallbackAPIVersion.VERSION2,
            client_id=f'sensor_publisher_{random.randint(10000, 99999)}'
        )
        self.client.on_connect = self.on_connect
        self.client.on_disconnect = self.on_disconnect
        
        try:
            self.client.connect(broker)
            self.client.loop_start()
            print(f"Подключено к MQTT брокеру {broker}")
        except Exception as e:
            print(f"Ошибка подключения к MQTT: {e}")
            self.ser.close()
            sys.exit(1)
    
    def on_connect(self, client, userdata, flags, reason_code, properties):
        if reason_code == 0:
            print("MQTT: Успешное подключение")
            self.client.publish(STATUS_TOPIC, "SENSOR_ONLINE", qos=1)
        else:
            print(f"MQTT: Не удалось подключиться, код: {reason_code}")
    
    def on_disconnect(self, client, userdata, flags, reason_code, properties):
        print(f"MQTT: Отключено, код: {reason_code}")
    
    def start_streaming(self):
        print("\nЗапуск потоковой передачи")
        self.ser.write(b's')
        time.sleep(0.1)
        
        if self.ser.in_waiting > 0:
            response = self.ser.readline().decode('utf-8').strip()
            print(f"MCU: {response}")
    
    def read_and_publish(self):
        try:
            while True:
                if self.ser.in_waiting > 0:
                    line = self.ser.readline().decode('utf-8').strip()
                    
                    if line.startswith("SENSOR_VALUE:"):
                        value = line.split(':')[1]
                        
                        result = self.client.publish(LUMINOSITY_TOPIC, value, qos=2)
                        
                        if result.rc == 0:
                            print(f"Опубликовано: {value} → {LUMINOSITY_TOPIC}")
                        else:
                            print(f"Ошибка публикации: {result.rc}")
                    else:
                        print(f"MCU: {line}")
                
                time.sleep(0.01) 
                
        except KeyboardInterrupt:
            print("\n\nОстановка по Ctrl+C ")
        finally:
            self.cleanup()
    
    def cleanup(self):
        print("\nОчистка ресурсов ")
        
        try:
            self.ser.write(b'x')
            time.sleep(0.1)
        except:
            pass
        
        try:
            self.client.publish(STATUS_TOPIC, "SENSOR_OFFLINE", qos=1)
            time.sleep(0.5)
        except:
            pass
        
        self.client.loop_stop()
        self.client.disconnect()
        self.ser.close()
        
        print("Ресурсы освобождены")

def main():
    print("PC1 - SENSOR PUBLISHER")
    
    publisher = SensorPublisher(SERIAL_PORT, BAUD_RATE, BROKER)
    
    time.sleep(1)
    
    publisher.start_streaming()
    
    print("\nНачало передачи данных ")
    print("(Нажмите Ctrl+C для остановки)\n")
    publisher.read_and_publish()

if __name__ == "__main__":
    main()