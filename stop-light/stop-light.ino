#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiClient.h>
#include "config.h"
#include "html.h"

const char* ssid = "TP-LINK_E582";
const char* password = HOME_WIFI_PASSWORD;
const int port = HTTP_PORT;

ESP8266WebServer server(port);

void rootPage() {
  server.send(200, "text/plain", HOME_page);
}

void setup() {

  Serial.begin(115200);
  Serial.println("----------------------------------");

  WiFi.begin(ssid, password);

  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.print("Connected to ");
  Serial.print(ssid);
  Serial.print(", IP Address: ");
  Serial.println(WiFi.localIP());

  server.on("/", rootPage);

  server.begin();
  Serial.print("Web Server Listening on Port ");
  Serial.println(port);
}

void loop() {
  server.handleClient();
}
