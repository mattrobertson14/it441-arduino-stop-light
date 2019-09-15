#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiClient.h>
#include "config.h"
#include "html.h"

const char* ssid = "TP-LINK_E582";
const char* password = HOME_WIFI_PASSWORD;
const int port = HTTP_PORT;

String color = "off";
bool cycle = false;
unsigned long currentTime;
unsigned long prevTime = 0;

ESP8266WebServer server(port);

void rootPage() {
  server.send(200, "text/html", HOME_page);
  logRequest("200", "");
}

void notFound() {
  server.sendHeader("Location", "/", true);
  server.send(302, "text/plain", "");
  logRequest("302", "Rerouted to \"/\"");
}

void getStatus() {
  server.send(200, "application/json", getColorJson(color));
}

void redOn() {
  cycle = false;
  String old_color = color;
  color = "red";
  String msg = "Changed light from " + old_color + " to " + color;
  server.send(200, "application/json", getColorJson(color));
  logRequest("200", msg);
}

void yellowOn() {
  cycle = false;
  String old_color = color;
  color = "yellow";
  String msg = "Changed light from " + old_color + " to " + color;
  server.send(200, "application/json", getColorJson(color));
  logRequest("200", msg);
}

void greenOn() {
  cycle = false;
  String old_color = color;
  color = "green";
  String msg = "Changed light from " + old_color + " to " + color;
  server.send(200, "application/json", getColorJson(color));
  logRequest("200", msg);
}

void turnOffLights() {
  cycle = false;
  String old_color = color;
  color = "off";
  String msg = "Changed light from " + old_color + " to " + color;
  server.send(200, "application/json", getColorJson(color));
  logRequest("200", msg);
}

void cycleLights() {
  color = "red";
  prevTime = millis();
  cycle = true;
  server.send(200, "application/json", getColorJson(color));
  logRequest("200", "Turned on Auto Mode");
}

String getColorJson(String _color) {
  String res = "{ \"color\": \"" + _color + "\"}";
  return res;
}

void logRequest(String request_status, String additional_info) {
  String clientIP = server.client().remoteIP().toString();
  String reqType = (server.method() == HTTP_GET)? "GET" : "POST";
  String reqURI = server.uri();
  String addInfoTabs = (reqURI.length() < 11)? "\t\t" : "\t";
  String msg = clientIP + "\t" + reqType + "\t" + request_status + "\t" + reqURI + addInfoTabs + additional_info;
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
  server.on("/api/lights-out", HTTP_POST, turnOffLights);
  server.on("/api/cycle", HTTP_POST, cycleLights);

  server.onNotFound(notFound);

  server.begin();
  Serial.print("Web Server Listening on Port ");
  Serial.println(port);
}

void loop() {
  server.handleClient();

  currentTime = (millis() - prevTime)/1000;

  if (cycle && currentTime < 5) color = "red";
  if (cycle && currentTime >= 5 && currentTime < 10) color = "green";
  if (cycle && currentTime >= 10 && currentTime < 12.5) color = "yellow";
  if (cycle && currentTime >= 12.5) prevTime = millis();
}
