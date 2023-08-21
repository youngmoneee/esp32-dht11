#include "DHT.h"
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <IRsend.h>


#define DHTTYPE  DHT11
#define DHTPIN   DHT_PIN_HOLE
#define IRPIN    IR_SEND_PIN_HOLE

const char* deviceName = DEVICENAME;
const char* tempTopic = DEVICENAME/temp;
const char* humidTopic = DEVICENAME/humid;
const char* inputTopic = DEVICENAME/input;

const char* ssid = WIFI_SSID;
const char* wifiPs = WIFI_PW;
const char* mqtt_server = YOUR_MQTT_SERVER;
const char* mqtt_port =   YOUR_MQTT_PORT;
unsigned long last = 0;

WiFiClient    espClient;
PubSubClient  client(espClient);
DHT           dht(DHTPIN, DHT11);
IRsend        ir(IRPIN);

void setup() {
  setWifi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
  dht.begin();
  ir.begin();
}

void loop() {
  if (!client.connected()) reconnect(client);
  client.loop();
  
  unsigned long now = millis();
  if (now - last > 5000) {
    last = now;
    char  temp[20];
    char  humid[20];
    float h = dht.readHumidity();
    float t = dht.readTemperature();
    if (!isnan(h) && !isnan(t)) {
      snprintf(temp, 20, "%.2f", t);
      snprintf(humid, 20, "%.2f", h);
      client.publish(tempTopic, temp);
      client.publish(humidTopic, humid);
    }
  }
}

void setWifi() {
  delay(100);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, wifiPs);
  while (WiFi.status() != WL_CONNECTED) delay(1000);
}

void reconnect(PubSubClient& client) {
  // Loop until we're reconnected
  while (!client.connected()) {
    String clientId = deviceName + String(random(0xffff), HEX);
    if (client.connect(clientId.c_str())) {
      client.subscribe("sungwon107/input");
    } else {
      delay(500);
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  char* tmp = (char *)malloc(sizeof(char) * (length + 1));
  tmp[length] = 0;
  if (strcmp(topic, inputTopi) == 0) {
    for (unsigned int i = 0; i < length; ++i) tmp[i] = (char)payload[i];
    //  if Command is on/off
    if (strcmp(tmp, "on") == 0)
      ir.sendLG(0x8800C40, 28);
    else if (strcmp(tmp, "off") == 0)
      ir.sendLG(0x88C0051, 28);
  }
  free(tmp);
}
