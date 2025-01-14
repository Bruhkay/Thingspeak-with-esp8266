#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>
//https://api.thingspeak.com/update?api_key=BBCNFT8TL919MV0I&field1=0 for write
//https://api.thingspeak.com/channels/2808305/feeds.json?api_key=8ERR6GQF0PP4S2PQ&results=2 for read
const char* ssid = "Maales";
const char* password = "burkay04";

const char* channelID = "2808305";
const char* api_keyWrite = "BBCNFT8TL919MV0I";
const char* api_keyRead = "8ERR6GQF0PP4S2PQ";
const char* base_url = "http://api.thingspeak.com/";

WiFiClient client;
HTTPClient http;

void connectToWiFi();
void sendGetRequest(int field, int data);
void receiveGetRequest(int field, int number);

void setup() {
  Serial.begin(115200);
  pinMode(D4, OUTPUT);
  connectToWiFi();
  sendGetRequest(2, 1227); // send 7 to field 1
  
}

void loop() {
  //sendGetRequest(2, analogRead(A0));
  receiveGetRequest(2, 1);
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
  //https://api.thingspeak.com/update?api_key=BBCNFT8TL919MV0I&field1=0
  String url = String(base_url) + "update?api_key=" + api_keyWrite + "&field" + field + "=" + data;
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

void receiveGetRequest(int field, int number){
  //https://api.thingspeak.com/channels/2808305/feeds.json?api_key=8ERR6GQF0PP4S2PQ&results=2
  String url = String(base_url) + "channels/"+ channelID+"/feeds.json?api_key=" + api_keyRead + "&results"+ "=" + number;
  http.begin(client, url);
  int httpCode = http.GET();

  if (httpCode > 0) {
    Serial.println("Data obtained");
    String response = http.getString().substring(http.getString().length()/2); //cutted from middle in order to eliminate insignificant part
    String header = "field" + String(field);
    int begin_index = response.indexOf(header)+ header.length() +3; // +3 for literals
    String result = "";
    for(int i = begin_index; response.charAt(i)!= '"';i++){
      result +=response.charAt(i);
    } 
    
    Serial.println("Last data from field "+ String(field) + " is: "+ String(result));
  } 
  else {
    Serial.println("Data couldnt be obtained");
  }
  http.end();
}