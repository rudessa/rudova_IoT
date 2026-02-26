const bool WIFI_START_MODE_CLIENT = false;
const bool WIFI_START_MODE_AP = true; // access point

String SSID_AP = "ME_WE_VESNIN";
String PASSWORD_AP = "12345678";
int led = LED_BUILTIN;

String SSID_CLI = "rudessa"; // set network
String SSID_PASSWORD_CLI = "qwerty12345";

char* mqtt_broker = "broker.emqx.io";
const int mqtt_port = 1883;