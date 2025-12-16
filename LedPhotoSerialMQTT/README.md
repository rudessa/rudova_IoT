# LedPhotoSerialMQTT


## Прошивка

```
Sensor.ino    → Arduino #1 
LED.ino  → Arduino #2 
```

## Настройка Python

### pc1_publisher.py

```
SERIAL_PORT = 'COM5'  # Порт Arduino #1
```

### pc2_subscriber.py
```
SERIAL_PORT = 'COM8'  # Порт Arduino #2
LIGHT_THRESHOLD = 150  
```

# Запуск 

## Терминал 1
```
python monitor.py
```
## Терминал 2
```
python pc1_publisher.py
```

## Терминал 3
```
python pc2_subscriber.py
```

## MQTT топики

```
laboratory/greenhouse/luminosity        # Данные (0-1023)
laboratory/greenhouse/sensor/status     # Статус датчика
laboratory/greenhouse/actuator/status   # Статус исполнителя
laboratory/greenhouse/light/status      # Статус света
```

## Логика работы

```
Освещённость < 500 → LED ON  (темно)
Освещённость ≥ 500 → LED OFF (светло)
```


## Порядок действий при сборке

1. ✅ Собрать схему Sensor MCU
2. ✅ Собрать схему Actuator MCU  
3. ✅ Прошить оба Arduino
4. ✅ Указать COM-порты в .py файлах
5. ✅ Запустить monitor.py
6. ✅ Запустить pc1_publisher.py
7. ✅ Запустить pc2_subscriber.py
8. ✅ Проверить: закрыть датчик рукой → LED включается

## Файлы проекта

| Файл | Назначение |
|------|------------|
| `Sensor.ino` | Arduino: считывание датчика |
| `LED.ino` | Arduino: управление LED |
| `pc1_publisher.py` | ПК: чтение MCU → MQTT |
| `pc2_subscriber.py` | ПК: MQTT → управление MCU |
| `monitor.py` | ПК: мониторинг системы |
