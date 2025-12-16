import serial
import time
from paho.mqtt.client import Client, MQTTMessage
from paho.mqtt.enums import CallbackAPIVersion
import random
import sys

SERIAL_PORT = 'COM8' 
BAUD_RATE = 9600
BROKER = "broker.emqx.io"
LUMINOSITY_TOPIC = "laboratory/greenhouse/luminosity"
LIGHT_STATUS_TOPIC = "laboratory/greenhouse/light/status"
ACTUATOR_STATUS_TOPIC = "laboratory/greenhouse/actuator/status"

LIGHT_THRESHOLD = 150


class ActuatorSubscriber:
    def __init__(self, serial_port, baud_rate, broker):
        self.serial_port = serial_port
        self.baud_rate = baud_rate
        self.broker = broker
        self.current_light_state = "OFF"

        try:
            self.ser = serial.Serial(serial_port, baud_rate, timeout=1)
            time.sleep(2)
            print(f"Подключено к {serial_port}")
        except serial.SerialException as e:
            print(f"Ошибка подключения к {serial_port}: {e}")
            sys.exit(1)

        self.client = Client(
            callback_api_version=CallbackAPIVersion.VERSION2,
            client_id=f'actuator_subscriber_{random.randint(10000, 99999)}'
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
            self.ser.close()
            sys.exit(1)

    def on_connect(self, client, userdata, flags, reason_code, properties):
        if reason_code == 0:
            print("MQTT: Успешное подключение")
            self.client.subscribe(LUMINOSITY_TOPIC, qos=2)
            print(f"Подписка на топик: {LUMINOSITY_TOPIC}")

            self.client.publish(ACTUATOR_STATUS_TOPIC,
                                "ACTUATOR_ONLINE", qos=1)
        else:
            print(f"MQTT: Не удалось подключиться, код: {reason_code}")

    def on_disconnect(self, client, userdata, flags, reason_code, properties):
        print(f"⚠ MQTT: Отключено, код: {reason_code}")

    def on_message(self, client, userdata, message: MQTTMessage):
        try:
            data = message.payload.decode("utf-8")
            topic = message.topic

            print(f"\nПолучено: {data} из {topic}")

            if topic == LUMINOSITY_TOPIC:
                self.process_luminosity(int(data))

        except Exception as e:
            print(f"Ошибка обработки сообщения: {e}")

    def process_luminosity(self, luminosity_value):
        print(f"Освещённость: {luminosity_value} (порог: {LIGHT_THRESHOLD})")

        if luminosity_value < LIGHT_THRESHOLD:
            if self.current_light_state != "ON":
                self.control_led('u')
                self.current_light_state = "ON"
                print("Действие: Включаем свет (темно)")
            else:
                print("Свет уже включён")
        else:
            if self.current_light_state != "OFF":
                self.control_led('d')
                self.current_light_state = "OFF"
                print("Действие: Выключаем свет (светло)")
            else:
                print("Свет уже выключен")

    def control_led(self, command):
        try:
            self.ser.write(command.encode())
            time.sleep(0.1)

            if self.ser.in_waiting > 0:
                response = self.ser.readline().decode('utf-8').strip()
                print(f"MCU ответ: {response}")

                status_msg = ""
                if response == "LED_GOES_ON":
                    status_msg = "ON"
                elif response == "LED_GOES_OFF":
                    status_msg = "OFF"
                elif response == "LED_WILL_BLINK":
                    status_msg = "BLINK"

                if status_msg:
                    self.client.publish(LIGHT_STATUS_TOPIC, status_msg, qos=1)
                    print(f"Статус света опубликован: {status_msg}")

        except Exception as e:
            print(f"Ошибка управления LED: {e}")

    def run(self):
        try:
            print("\n Начало работы ")
            print("(Нажмите Ctrl+C для остановки)\n")

            while True:
                time.sleep(0.1)

        except KeyboardInterrupt:
            print("\n\n Остановка по Ctrl+C ")
        finally:
            self.cleanup()

    def cleanup(self):
        print("\n Очистка ресурсов ")

        try:
            self.control_led('d')
        except:
            pass

        try:
            self.client.publish(ACTUATOR_STATUS_TOPIC,
                                "ACTUATOR_OFFLINE", qos=1)
            self.client.publish(LIGHT_STATUS_TOPIC, "OFF", qos=1)
            time.sleep(0.5)
        except:
            pass

        self.client.loop_stop()
        self.client.disconnect()
        self.ser.close()

        print("Ресурсы освобождены")


def main():
    print("PC2 - ACTUATOR SUBSCRIBER")
    print(f"Порог освещённости: {LIGHT_THRESHOLD}")
    print(f"  < {LIGHT_THRESHOLD} → LED включён (темно)")
    print(f"  ≥ {LIGHT_THRESHOLD} → LED выключен (светло)")

    subscriber = ActuatorSubscriber(SERIAL_PORT, BAUD_RATE, BROKER)

    time.sleep(1)

    subscriber.run()


if __name__ == "__main__":
    main()
