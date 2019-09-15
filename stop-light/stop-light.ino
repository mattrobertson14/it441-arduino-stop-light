#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiClient.h>
#include "config.h"
#include "html.h"

const char* ssid = "TP-LINK_E582";
const char* password = HOME_WIFI_PASSWORD;
const int port = HTTP_PORT;

String color = "off";

ESP8266WebServer server(port);

void rootPage() {
  server.send(200, "text/html", HOME_page);
}

void getStatus() {
  server.send(200, "application/json", getColorJson(color));
}

void redOn() {
  color = "red";
  Serial.println("Turning red light on");
  server.send(200, "application/json", getColorJson(color));
}

void yellowOn() {
  color = "yellow";
  Serial.println("Turning yellow light on");
  server.send(200, "application/json", getColorJson(color));
}

void greenOn() {
  color = "green";
  Serial.println("Turning green light on");
  server.send(200, "application/json", getColorJson(color));
}

String getColorJson(String _color) {
  String res = "{ \"color\": \"" + _color + "\"}";
  return res;
}

void logRequest(String request_type, String request, String request_status) {
  String clientIP = server.client().remoteIP().toString();
  String msg = request_type + "\t" + request_status + "\t" + request;
  Serial.println(msg);
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

  server.on("/", HTTP_GET, rootPage);
  server.on("/api/status", HTTP_GET, getStatus);
  server.on("/api/set-red", HTTP_POST, redOn);
  server.on("/api/set-yellow", HTTP_POST, yellowOn);
  server.on("/api/set-green", HTTP_POST, greenOn);

  server.begin();
  Serial.print("Web Server Listening on Port ");
  Serial.println(port);
}

void loop() {
  server.handleClient();
}
