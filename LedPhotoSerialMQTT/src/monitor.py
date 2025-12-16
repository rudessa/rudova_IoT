import time
from paho.mqtt.client import Client, MQTTMessage
from paho.mqtt.enums import CallbackAPIVersion
import random
from datetime import datetime
import sys

BROKER = "broker.emqx.io"
BASE_TOPIC = "laboratory/greenhouse/#"  

class SystemMonitor:
    def __init__(self, broker):
        self.broker = broker
        self.message_count = 0
        self.sensor_online = False
        self.actuator_online = False
        self.last_luminosity = None
        self.last_light_status = None
        
        self.client = Client(
            callback_api_version=CallbackAPIVersion.VERSION2,
            client_id=f'system_monitor_{random.randint(10000, 99999)}'
        )
        self.client.on_connect = self.on_connect
        self.client.on_message = self.on_message
        self.client.on_disconnect = self.on_disconnect
        
        try:
            self.client.connect(broker)
            self.client.loop_start()
            print(f"Подключено к MQTT брокеру {broker}")
        except Exception as e:
            print(f"Ошибка подключения к MQTT: {e}")
            sys.exit(1)
    
    def on_connect(self, client, userdata, flags, reason_code, properties):
        if reason_code == 0:
            print("MQTT: Успешное подключение")
            self.client.subscribe(BASE_TOPIC, qos=2)
            print(f"Подписка на топики: {BASE_TOPIC}")
        else:
            print(f"MQTT: Не удалось подключиться, код: {reason_code}")
    
    def on_disconnect(self, client, userdata, flags, reason_code, properties):
        print(f"MQTT: Отключено, код: {reason_code}")
    
    def on_message(self, client, userdata, message: MQTTMessage):
        self.message_count += 1
        timestamp = datetime.now().strftime("%H:%M:%S")
        data = message.payload.decode("utf-8")
        topic = message.topic
        
        color_reset = "\033[0m"
        
        if "luminosity" in topic:
            color = "\033[96m"  
            self.last_luminosity = int(data)
        elif "status" in topic:
            color = "\033[93m" 
            self.update_system_status(topic, data)
        else:
            color = "\033[92m" 
            if "light" in topic:
                self.last_light_status = data
        
        print(f"{color}[{timestamp}] [{self.message_count:04d}] {topic}")
        print(f"  └─ {data}{color_reset}")
        
        if self.message_count % 10 == 0:
            self.print_system_status()
    
    def update_system_status(self, topic, data):
        if "sensor" in topic:
            if data == "SENSOR_ONLINE":
                self.sensor_online = True
            elif data == "SENSOR_OFFLINE":
                self.sensor_online = False
        elif "actuator" in topic:
            if data == "ACTUATOR_ONLINE":
                self.actuator_online = True
            elif data == "ACTUATOR_OFFLINE":
                self.actuator_online = False
    
    def print_system_status(self):
        
        print(f" Датчик (Sensor MCU): {'ONLINE' if self.sensor_online else 'OFFLINE'}")
        
        print(f"Исполнитель (Actuator MCU): {'ONLINE' if self.actuator_online else 'OFFLINE'}")
        
        if self.last_luminosity is not None:
            print(f"💡 Последняя освещённость: {self.last_luminosity}")
        else:
            print(f"💡 Последняя освещённость: N/A")
        
        if self.last_light_status:
            light_emoji = "🔆" if self.last_light_status == "ON" else "🌙"
            print(f"{light_emoji} Статус света: {self.last_light_status}")
        else:
            print(f"Статус света: N/A")
        
        print("\nПРОВЕРКА СИСТЕМЫ:")
        
        if self.sensor_online and self.actuator_online:
            print("Оба компонента в сети")
        else:
            if not self.sensor_online:
                print("Датчик не в сети!")
            if not self.actuator_online:
                print("Исполнитель не в сети!")
        
        if self.last_luminosity is not None and self.last_light_status:
            threshold = 500 
            expected_status = "ON" if self.last_luminosity < threshold else "OFF"
            
            if self.last_light_status == expected_status:
                print(f"Свет соответствует освещённости (ожидалось: {expected_status})")
            else:
                print(f"Несоответствие! Освещённость: {self.last_luminosity}, ")
                print(f"Ожидалось: {expected_status}, Фактически: {self.last_light_status}")
        
        print(f"\nВсего сообщений обработано: {self.message_count}")
    
    def run(self):
        try:
            print("МОНИТОР СИСТЕМЫ ЗАПУЩЕН")
            print("Отслеживаются все топики: laboratory/greenhouse/#")
            print("(Нажмите Ctrl+C для остановки)\n")
            
            while True:
                time.sleep(1)
                
        except KeyboardInterrupt:
            print("\n\n Остановка по Ctrl+C ")
        finally:
            self.cleanup()
    
    def cleanup(self):
        print("\n Очистка ресурсов ")
        
        self.print_system_status()
        
        self.client.loop_stop()
        self.client.disconnect()
        
        print("Монитор остановлен")

def main():
    print("SYSTEM MONITOR")
    
    monitor = SystemMonitor(BROKER)
    
    time.sleep(1)
    
    monitor.run()

if __name__ == "__main__":
    main()