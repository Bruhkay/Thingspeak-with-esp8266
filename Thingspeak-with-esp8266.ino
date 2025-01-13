#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>

const char* ssid = "Galaxy";
const char* password = "burkay04";

const char* api_key = "BBCNFT8TL919MV0I";
const char* base_url = "http://api.thingspeak.com/update";

WiFiClient client;
HTTPClient http;

void connectToWiFi();
void sendGetRequest(int field, int data);

void setup() {
  Serial.begin(115200);
  connectToWiFi();
  sendGetRequest(2, 7); // send 7 to field 1
  pinMode(D4, OUTPUT);
    
}

void loop() {
  sendGetRequest(2, analogRead(A0));
  delay(16000);
}

void connectToWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting");

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(200);
  }

  Serial.println("\nWiFi connected");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  Serial.print("MAC Address: ");
  Serial.println(WiFi.macAddress());
}
void sendGetRequest(int field, int data) {
  String url = String(base_url) + "?api_key=" + api_key + "&field" + field + "=" + data;
  http.begin(client, url);
  int httpCode = http.GET();

  if (httpCode > 0) {
    digitalWrite(D4, LOW);
    delay(100);
    digitalWrite(D4, HIGH);
    Serial.println("Data sent succesfully");
  } 
  else {
    Serial.println("Data couldnt be sent");
  }
  http.end();
}