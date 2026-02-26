#include "Config.h"
#include "WIFI.h"
#include "Server.h"
#include "MQTT.h"

void setup() {
    Serial.begin(115200);
    init_WIFI(WIFI_START_MODE_CLIENT);
    delay(500);
    Serial.println("Our id is:");
    Serial.println(id());
    init_server();
    init_MQTT();
    mqtt_cli.subcribe("esp82266/command");

}

void loop() {
    server.handleClient();
    mqtt_cli.loop();


}