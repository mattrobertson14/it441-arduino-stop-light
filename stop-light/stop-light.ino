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
  String old_color = color;
  color = "red";
  String msg = "Changed light from " + old_color + " to " + color;
  server.send(200, "application/json", getColorJson(color));
  logRequest("200", msg);
}

void yellowOn() {
  String old_color = color;
  color = "yellow";
  String msg = "Changed light from " + old_color + " to " + color;
  server.send(200, "application/json", getColorJson(color));
  logRequest("200", msg);
}

void greenOn() {
  String old_color = color;
  color = "green";
  String msg = "Changed light from " + old_color + " to " + color;
  server.send(200, "application/json", getColorJson(color));
  logRequest("200", msg);
}

void turnOffLights() {
  String old_color = color;
  color = "off";
  String msg = "Changed light from " + old_color + " to " + color;
  server.send(200, "application/json", getColorJson(color));
  logRequest("200", msg);
}

String getColorJson(String _color) {
  String res = "{ \"color\": \"" + _color + "\"}";
  return res;
}

void logRequest(String request_status, String additional_info) {
  String clientIP = server.client().remoteIP().toString();
  String reqType = (server.method() == HTTP_GET)? "GET" : "POST";
  String reqURI = server.uri();
  String msg = clientIP + "\t" + reqType + "\t" + request_status + "\t" + reqURI + "\t" + additional_info;
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

  server.onNotFound(notFound);

  server.begin();
  Serial.print("Web Server Listening on Port ");
  Serial.println(port);
}

void loop() {
  server.handleClient();
}
