#include <WiFi.h>
#include <PubSubClient.h>
#include <wifi/WiFiHandler.h>


const char* ssid = "Hotspot";
const char* password = "heineken123";


const char* mqtt_server = "192.168.41.156";
const int mqtt_port = 1885;
const char* mqtt_user = "default";
const char* mqtt_password = "default";


const char* user_id = "123e4567-e89b-12d3-a456-426614174000";
const char* device_id = "device_1";


WiFiClient espClient;
PubSubClient client(espClient);
WiFiHandler wifiHandler(ssid, password);


// Funkcja do łączenia z brokerem MQTT
void reconnect() {
  while (!client.connected()) {
    Serial.print("Łączenie z brokerem MQTT...");
    if (client.connect(device_id, mqtt_user, mqtt_password)) {
      Serial.println("Połączono");
      String statusTopic = String("/users/") + user_id + "/devices/" + device_id + "/status";
      client.subscribe(statusTopic.c_str());
    } else {
      Serial.print("Błąd: ");
      Serial.print(client.state());
      delay(2000);
    }
  }
}

void setup() {
  Serial.begin(9600);
  wifiHandler.connectToWiFi();
  client.setServer(mqtt_server, mqtt_port);
}

void loop() {
  wifiHandler.monitorConnection();
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // Symulacja odczytu z czujnika
  float temperature = 25.0 + random(-100, 100) / 100.0;

  // Publikowanie danych
  String topic = String("/users/") + user_id + "/devices/" + device_id + "/temperature";
  String payload = String("{\"temperature\":") + temperature + "}";
  client.publish(topic.c_str(), payload.c_str());

  delay(5000); // Wysyłaj dane co 5 sekund
}
