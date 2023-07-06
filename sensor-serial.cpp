#include "DHT.h"
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <IRsend.h>

#define DHTPIN D3  //  온습도
#define DHTTYPE DHT11
#define IRPIN 4    //  적외선

const char* tempTopic = TEMP_TOPIC_NAME;
const char* humidTopic = HUMID_TOPIC_NAME;
const char* deviceName = DEVICE_NAME;
const char* ssid = WIFI_SSID;
const char* wifiPs = WIFI_PASSWD;
const char* mqtt_server = MQTT_ADDR;
unsigned long last = 0;

WiFiClient espClient;
PubSubClient client(espClient);
DHT dht(DHTPIN, DHT11);
IRsend ir(IRPIN);

void setup() {
  setWifi();
  client.setServer(mqtt_server, 1883);
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
      snprintf(temp, 20, "%f", t);
      snprintf(humid, 20, "%f", h);
      client.publish(tempTopic, temp);
      client.publish(humidTopic, humid);
    }
  }
}

void setWifi() {
  delay(10);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, wifiPs);
  while (WiFi.status() != WL_CONNECTED) delay(500);
}

void reconnect(PubSubClient& client) {
  // Loop until we're reconnected
  while (!client.connected()) {
    if (client.connect(deviceName)) {
      client.subscribe("dev1/#");
    } else {
      delay(500);
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  String tmp;
  for (unsigned int i = 0; i < length; ++i) tmp += (char)payload[i];
  if (strcmp(topic, "dev1/off") == 0) {
    ir.sendLG(0x88C0051, 28);
  }
  if (strcmp(topic, "dev1/on") == 0) {
    ir.sendLG(0x8800A0A, 28);
  }
}
